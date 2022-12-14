#include "gkernel/area_analyzer.hpp"
#include "gkernel/rbtree.hpp"

namespace gkernel {

static constexpr label_data_type unchecked_segment = -2;
static constexpr label_data_type unassigned = -1;

enum event_status {
    vertical,
    start,
    end
};

struct Event {
    Event(double x, const Segment* segment, event_status status) : x(x), segment(segment), status(status) {}
    double x;
    const Segment* segment;
    event_status status;
};

enum find_neighbours_label_type {
    circuits_layer_id = 0,
    top,
    bottom
};

enum mark_areas_label_type {
    first_circuits_layer_top = 0,
    second_circuits_layer_top,
    first_circuits_layer_bottom,
    second_circuits_layer_bottom
};

SegmentsLayer AreaAnalyzer::findSegmentsNeighbours(const SegmentsLayer& layer) {
    std::vector<Event> events;
    events.reserve(layer.size() * 2);

    std::vector<Segment> temp_result;
    temp_result.reserve(layer.size());

    for (std::size_t idx = 0; idx < layer.size(); ++idx) {
        const Segment& segment = layer[idx];
        temp_result.emplace_back(segment);
    }

    SegmentsSet result(std::move(temp_result));

    for (std::size_t idx = 0; idx < result.size(); ++idx) {
        const Segment& segment = result[idx];
        if (segment.start().x() != segment.end().x()) {
            events.emplace_back(segment.min().x(), &segment, event_status::start);
            events.emplace_back(segment.max().x(), &segment, event_status::end);
        } else {
            events.emplace_back(segment.min().x(), &segment, event_status::vertical);
        }
    }


    std::sort(events.begin(), events.end(), [](const Event& lhs, const Event& rhs) {
        if (lhs.x == rhs.x) {
            if (lhs.status == rhs.status) {
                return false;
            } else {
                return static_cast<int>(lhs.status) > static_cast<int>(rhs.status);
            }
        }
        return lhs.x < rhs.x;
    });

    result.set_labels_types({ find_neighbours_label_type::circuits_layer_id, find_neighbours_label_type::top, find_neighbours_label_type::bottom });
    for (std::size_t idx = 0; idx < layer.size(); ++idx) {
        result.set_label_value(find_neighbours_label_type::circuits_layer_id, result[idx], layer.get_label_value(find_neighbours_label_type::circuits_layer_id, layer[idx]));
    }

    std::vector<label_data_type> default_labels_values(result.size());
    std::fill(default_labels_values.begin(), default_labels_values.end(), unchecked_segment);
    result.set_label_values(find_neighbours_label_type::top, default_labels_values);
    result.set_label_values(find_neighbours_label_type::bottom, default_labels_values);

    double x_sweeping_line = 0;

    auto compare_segments = [&x_sweeping_line](const Segment* first, const Segment* second) -> bool {
        double y1 = get_sweeping_line_y(*first, x_sweeping_line);
        double y2 = get_sweeping_line_y(*second, x_sweeping_line);
        if (y1 != y2) {
            return y1 < y2;
        } else {
            return first->id < second->id;
        }
    };

    using tree_type = RBTree<const Segment*, decltype(compare_segments)>;
    tree_type active_segments(compare_segments);

    auto current_event = events.begin();

    while (current_event != events.end()) {
        double x_sweeping_line_new = current_event->x;

        while (current_event != events.end() && current_event->x == x_sweeping_line_new) {
            if (current_event->status == event_status::start) {
                x_sweeping_line = x_sweeping_line_new;
                active_segments.insert(current_event->segment);
            } else if (current_event->status == event_status::end) {
                active_segments.erase(current_event->segment);
            } else {
                result.set_label_value(find_neighbours_label_type::top, *current_event->segment, unassigned);
                result.set_label_value(find_neighbours_label_type::bottom, *current_event->segment, unassigned);

                auto next_id = -1;
                auto prev_id = -1;
                auto active_iter = active_segments.end();
                for (auto idx = active_iter; idx != active_segments.begin(); idx--) {
                    --active_iter;
                    if (next_id != -1 && prev_id != -1) {
                        break;
                    }
                    if (current_event->segment->max().y() <= (**active_iter).min().y()) {
                        next_id = (**active_iter).id;
                    }
                    else if (current_event->segment->min().y() >= (**active_iter).min().y() && prev_id == -1) {
                        prev_id = (**active_iter).id;
                    } else break;
                }
                if (next_id != -1) {
                    result.set_label_value(find_neighbours_label_type::top, *current_event->segment, next_id);
                }
                if (prev_id != -1) {
                    result.set_label_value(find_neighbours_label_type::bottom, *current_event->segment, prev_id);
                }
            }
            ++current_event;
        }

        auto current_segment = active_segments.begin();
        while (current_segment != active_segments.end()) {
            // if current_segment is already processed then skip
            if (result.get_label_value(find_neighbours_label_type::top, **current_segment) != unchecked_segment || result.get_label_value(find_neighbours_label_type::bottom, **current_segment) != unchecked_segment) {
                ++current_segment;
                continue;
            }

            result.set_label_value(find_neighbours_label_type::top, **current_segment, unassigned);
            result.set_label_value(find_neighbours_label_type::bottom, **current_segment, unassigned);

            auto next_segment = current_segment;
            ++next_segment;
            if (next_segment != active_segments.end()) {
                result.set_label_value(find_neighbours_label_type::top, **current_segment, (**next_segment).id);
            }

            auto prev_segment = current_segment;
            if (current_segment != active_segments.begin()) {
                --prev_segment;
                result.set_label_value(find_neighbours_label_type::bottom, **current_segment, (**prev_segment).id);
            }

            ++current_segment;
        }
    }

    return static_cast<SegmentsLayer>(result);
}

SegmentsLayer AreaAnalyzer::markAreas(const SegmentsLayer& layer) {
    std::vector<Segment> temp_result;
    temp_result.reserve(layer.size());

    for (std::size_t idx = 0; idx < layer.size(); ++idx) {
        const Segment& segment = layer[idx];
        temp_result.emplace_back(segment);
    }

    SegmentsSet result(std::move(temp_result));


    result.set_labels_types({ mark_areas_label_type::first_circuits_layer_top, mark_areas_label_type::second_circuits_layer_top,
                                mark_areas_label_type::first_circuits_layer_bottom, mark_areas_label_type::second_circuits_layer_bottom });

    for (std::size_t idx = 0; idx < result.size(); ++idx) {
        label_data_type top = layer.get_label_value(find_neighbours_label_type::top, layer[idx]);
        bool first_circuits_layer_top = false;
        bool second_circuits_layer_top = false;

        while (top != unassigned) {
            if (layer.get_label_value(find_neighbours_label_type::circuits_layer_id, layer[top]) == 0) {
                first_circuits_layer_top ^= true;
            } else {
                second_circuits_layer_top ^= true;
            }

            top = layer.get_label_value(find_neighbours_label_type::top, layer[top]);
        }

        label_data_type bottom = layer.get_label_value(find_neighbours_label_type::bottom, layer[idx]);
        bool first_circuits_layer_bottom = false;
        bool second_circuits_layer_bottom = false;

        while (bottom != unassigned) {
            if (layer.get_label_value(find_neighbours_label_type::circuits_layer_id, layer[bottom]) == 0) {
                first_circuits_layer_bottom ^= true;
            } else {
                second_circuits_layer_bottom ^= true;
            }

            bottom = layer.get_label_value(find_neighbours_label_type::bottom, layer[bottom]);
        }

        result.set_label_value(mark_areas_label_type::first_circuits_layer_top, result[idx], first_circuits_layer_top);
        result.set_label_value(mark_areas_label_type::second_circuits_layer_top, result[idx], second_circuits_layer_top);
        result.set_label_value(mark_areas_label_type::first_circuits_layer_bottom, result[idx], first_circuits_layer_bottom);
        result.set_label_value(mark_areas_label_type::second_circuits_layer_bottom, result[idx], second_circuits_layer_bottom);
    }

    return result;
}

} // namespace gkernel
