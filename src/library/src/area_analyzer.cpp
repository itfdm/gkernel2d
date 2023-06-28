#include "gkernel/area_analyzer.hpp"
#include "gkernel/rbtree.hpp"

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

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
        double y1;
        double y2;
        if (first->max().x() > x_sweeping_line) {
            y1 = get_sweeping_line_y(*first, x_sweeping_line, EPS);
        }
        else {
            y1 = get_sweeping_line_y(*first, x_sweeping_line, -EPS);
        }
        if (second->max().x() > x_sweeping_line) {
            y2 = get_sweeping_line_y(*second, x_sweeping_line, EPS);
        }
        else {
            y2 = get_sweeping_line_y(*second, x_sweeping_line, -EPS);
        }
        if (y1 != y2) {
            return y1 < y2;
        } else {
            return first->id > second->id;
        }
    };

    using tree_type = RBTree<const Segment*, decltype(compare_segments)>;
    tree_type active_segments(compare_segments);

    auto current_event = events.begin();

    std::vector<tree_type::iterator> active_segments_new;
    active_segments_new.reserve(events.size());

    while (current_event != events.end()) {
        double x_sweeping_line_new = current_event->x;

        while (current_event != events.end() && current_event->x == x_sweeping_line_new) {
            if (current_event->status == event_status::start) {
                x_sweeping_line = x_sweeping_line_new;
                auto insert_result = active_segments.insert(current_event->segment);
                active_segments_new.push_back(insert_result.first);
            } else if (current_event->status == event_status::end) {
                active_segments.erase(current_event->segment);
            } else {
                auto lower_bound = Segment(Point{current_event->segment->min().x() - 10 * EPS, current_event->segment->min().y() + 10 * EPS},
                                           Point{current_event->segment->min().x() + 10 * EPS, current_event->segment->min().y() + 10 * EPS});
                auto upper_bound = Segment(Point{current_event->segment->max().x() - 10 * EPS, current_event->segment->max().y() - 10 * EPS},
                                           Point{current_event->segment->max().x() + 10 * EPS, current_event->segment->max().y() - 10 * EPS});
                auto prev_segment = active_segments.find_prev(&lower_bound);
                auto next_segment = active_segments.find_next(&upper_bound);
                if (prev_segment.first != active_segments.end()) {
                    double prev_y = get_sweeping_line_y(**prev_segment.first, x_sweeping_line);
                    if (prev_y <= current_event->segment->min().y()) {
                        result.set_label_value(find_neighbours_label_type::bottom, *(current_event->segment), (*prev_segment.first)->id);
                    } else {
                        result.set_label_value(find_neighbours_label_type::bottom, *(current_event->segment), unassigned);
                    }
                }
                if (next_segment.first != active_segments.end()) {
                    result.set_label_value(find_neighbours_label_type::top, *(current_event->segment), (*next_segment.first)->id);
                } else {
                    result.set_label_value(find_neighbours_label_type::top, *(current_event->segment), unassigned);
                }
            }

            ++current_event;
        }

        for (auto current_segment : active_segments_new) {
            result.set_label_value(find_neighbours_label_type::top, **current_segment, unassigned);
            result.set_label_value(find_neighbours_label_type::bottom, **current_segment, unassigned);

            auto next_segment = current_segment;
            ++next_segment;
            if (next_segment != active_segments.end()) {
                result.set_label_value(find_neighbours_label_type::top, **current_segment, (**next_segment).id);
            }

            auto prev_segment = current_segment;
            if (prev_segment != active_segments.begin()) {
                --prev_segment;
                result.set_label_value(find_neighbours_label_type::bottom, **current_segment, (**prev_segment).id);
            }

            ++current_segment;
        }
        active_segments_new.clear();
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

    auto label_values_id = layer.get_label_values(find_neighbours_label_type::circuits_layer_id);
    auto label_values_top = layer.get_label_values(find_neighbours_label_type::top);
    auto label_values_bottom = layer.get_label_values(find_neighbours_label_type::bottom);

    tbb::parallel_for(tbb::blocked_range<size_t>(0, result.size()), [&](const auto &range) {
        for (std::size_t idx = range.begin(); idx < range.end(); ++idx) {
            label_data_type top = label_values_top[layer[idx].id];
            bool first_circuits_layer_top = false;
            bool second_circuits_layer_top = false;

            while (top != unassigned) {
                if (label_values_id[layer[top].id] == 0) {
                    first_circuits_layer_top ^= true;
                } else {
                    second_circuits_layer_top ^= true;
                }
                top = label_values_top[layer[top].id];
            }

            label_data_type bottom = label_values_bottom[layer[idx].id];
            bool first_circuits_layer_bottom = false;
            bool second_circuits_layer_bottom = false;

            while (bottom != unassigned) {
                if (label_values_id[layer[bottom].id] == 0) {
                    first_circuits_layer_bottom ^= true;
                } else {
                    second_circuits_layer_bottom ^= true;
                }

                bottom = label_values_bottom[layer[bottom].id];
            }

            result.set_label_value(mark_areas_label_type::first_circuits_layer_top, result[idx], first_circuits_layer_top);
            result.set_label_value(mark_areas_label_type::second_circuits_layer_top, result[idx], second_circuits_layer_top);
            result.set_label_value(mark_areas_label_type::first_circuits_layer_bottom, result[idx], first_circuits_layer_bottom);
            result.set_label_value(mark_areas_label_type::second_circuits_layer_bottom, result[idx], second_circuits_layer_bottom);
        }
    });

    return result;
}
} // namespace gkernel
