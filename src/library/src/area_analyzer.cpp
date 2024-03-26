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

void AreaAnalyzer::internalFindSegmentsNeighbours(const SegmentsLayer& layer, SegmentsSet& result, bool rotated = false) {
    std::vector<Event> events;
    events.reserve(layer.size() * 2);
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
    std::vector<const Segment*> active_segments_new;
    active_segments_new.reserve(events.size());

    auto current_event = events.begin();
    while (current_event != events.end()) {
        double x_sweeping_line_new = current_event->x;

        while (current_event != events.end() && current_event->x == x_sweeping_line_new) {
            if (current_event->status == event_status::start) {
                x_sweeping_line = x_sweeping_line_new;
                auto insert_result = active_segments.insert(current_event->segment);
                active_segments_new.push_back(current_event->segment);
            } else if (current_event->status == event_status::end) {
                active_segments.erase(current_event->segment);
            } else {
                ++current_event;
                continue;
            }

            ++current_event;
        }

        for (auto current_segment : active_segments_new) {
            auto current_segment_iter = active_segments.find(current_segment);
            result.set_label_value(find_neighbours_label_type::top, **current_segment_iter, unassigned);
            result.set_label_value(find_neighbours_label_type::bottom, **current_segment_iter, unassigned);

            auto next_segment = current_segment_iter;
            ++next_segment;
            if (next_segment != active_segments.end()) {
                if (!rotated) {
                    result.set_label_value(find_neighbours_label_type::top, **current_segment_iter, (**next_segment).id);
                } else {
                    result.set_label_value(find_neighbours_label_type::bottom, **current_segment_iter, (**next_segment).id);
                }
            }

            auto prev_segment = current_segment_iter;
            if (prev_segment != active_segments.begin()) {
                --prev_segment;
                if (!rotated) {
                    result.set_label_value(find_neighbours_label_type::bottom, **current_segment_iter, (**prev_segment).id);
                } else {
                    result.set_label_value(find_neighbours_label_type::top, **current_segment_iter, (**prev_segment).id);
                }
            }
        }
        active_segments_new.clear();
    }
}

std::pair<SegmentsSet, SegmentsSet> AreaAnalyzer::findSegmentsNeighbours(const SegmentsLayer& layer) {
    std::vector<Segment> temp_result;
    temp_result.reserve(layer.size());

    for (std::size_t idx = 0; idx < layer.size(); ++idx) {
        const Segment& segment = layer[idx];
        temp_result.emplace_back(segment);
    }

    SegmentsSet result(std::move(temp_result));

    result.set_labels_types({ find_neighbours_label_type::circuits_layer_id, find_neighbours_label_type::top, find_neighbours_label_type::bottom });
    for (std::size_t idx = 0; idx < layer.size(); ++idx) {
        result[idx].id = layer[idx].id;
        result.set_label_value(find_neighbours_label_type::circuits_layer_id, result[idx], layer.get_label_value(find_neighbours_label_type::circuits_layer_id, layer[idx]));
    }

    std::vector<label_data_type> default_labels_values(result.size());
    std::fill(default_labels_values.begin(), default_labels_values.end(), unchecked_segment);
    result.set_label_values(find_neighbours_label_type::top, default_labels_values);
    result.set_label_values(find_neighbours_label_type::bottom, default_labels_values);

    AreaAnalyzer::internalFindSegmentsNeighbours(layer, result);

    // rotate
    SegmentsSet result_rotated(result);
    result_rotated.set_label_values(find_neighbours_label_type::top, default_labels_values);
    result_rotated.set_label_values(find_neighbours_label_type::bottom, default_labels_values);

    for (std::size_t idx = 0; idx < result.size(); ++idx) {
        result_rotated[idx].rotate();
    }

    for (std::size_t idx = 0; idx < layer.size(); ++idx) {
        result_rotated[idx].id = layer[idx].id;
    }

    AreaAnalyzer::internalFindSegmentsNeighbours(layer, result_rotated, true);

    return std::make_pair(result, result_rotated);
}

void AreaAnalyzer::bypassNeighbours(std::vector<gkernel::label_data_type>& neighbours, std::vector<std::size_t>& history, std::vector<gkernel::label_data_type>& segment_layer_ids,
        SegmentsSet& result, gkernel::label_data_type start_idx, direction direction) {
    label_data_type neighbour_id = neighbours[result[start_idx].id];
    history.push_back(result[start_idx].id);

    bool start_is_vertical = result[start_idx].is_vertical();
    auto first_marker = direction == direction::top ? mark_areas_label_type::first_circuits_layer_top : mark_areas_label_type::first_circuits_layer_bottom;
    auto second_marker = direction == direction::top ? mark_areas_label_type::second_circuits_layer_top : mark_areas_label_type::second_circuits_layer_bottom;

    bool is_break = false;
    while (neighbour_id != unassigned) {
        history.push_back(neighbour_id);
        if (result[neighbour_id].is_vertical() == start_is_vertical && result.get_label_value(first_marker, result[neighbour_id]) != unassigned) {
            is_break = true;
            break;
        }
        neighbour_id = neighbours[neighbour_id];
    }

    if (!is_break && (result.get_by_id(history.back()).is_vertical() ==
            start_is_vertical)) {
        result.set_label_value(first_marker, result.get_by_id(history.back()), false);
        result.set_label_value(second_marker, result.get_by_id(history.back()), false);
    }

    if (history.size() > 1) {
        bool first_circuits_layer_side = false;
        bool second_circuits_layer_side = false;
        if (is_break) {
            first_circuits_layer_side = result.get_label_value(first_marker, result.get_by_id(history.back()));
            second_circuits_layer_side = result.get_label_value(second_marker, result.get_by_id(history.back()));
        }
        for (auto idx_iter = history.rbegin() + 1; idx_iter != history.rend(); ++idx_iter) {
            auto& segment = result.get_by_id(*idx_iter);
            if (segment_layer_ids[*(idx_iter - 1)] == 0) {
                first_circuits_layer_side ^= true;
            } else if (segment_layer_ids[*(idx_iter - 1)] == 1) {
                second_circuits_layer_side ^= true;
            } else {
                first_circuits_layer_side ^= true;
                second_circuits_layer_side ^= true;
            }
            if (segment.is_vertical() ==
                    start_is_vertical) {
                result.set_label_value(first_marker, segment, first_circuits_layer_side);
                result.set_label_value(second_marker, segment, second_circuits_layer_side);
            }
        }
    }
}

SegmentsLayer AreaAnalyzer::markAreas(const std::pair<SegmentsSet, SegmentsSet>& neighbours) {
    std::vector<Segment> temp_result;
    auto layer = neighbours.first;
    auto layer_rotated = neighbours.second;

    temp_result.reserve(layer.size());

    for (std::size_t idx = 0; idx < layer.size(); ++idx) {
        const Segment& segment = layer[idx];
        temp_result.emplace_back(segment);
    }

    SegmentsSet result(std::move(temp_result));

    for (std::size_t idx = 0; idx < layer.size(); ++idx) {
        result[idx].id = layer[idx].id;
    }

    result.set_labels_types({ mark_areas_label_type::first_circuits_layer_top, mark_areas_label_type::second_circuits_layer_top,
                              mark_areas_label_type::first_circuits_layer_bottom, mark_areas_label_type::second_circuits_layer_bottom });

    for (std::size_t idx = 0; idx < result.size(); ++idx) {
        result.set_label_value(mark_areas_label_type::first_circuits_layer_top, result[idx], unassigned);
        result.set_label_value(mark_areas_label_type::first_circuits_layer_bottom, result[idx], unassigned);
    }

    auto& circuit_layer_id = layer.get_label_values(find_neighbours_label_type::circuits_layer_id);
    auto& label_values_top = layer.get_label_values(find_neighbours_label_type::top);
    auto& label_values_bottom = layer.get_label_values(find_neighbours_label_type::bottom);

    auto& label_values_top_rotated = layer_rotated.get_label_values(find_neighbours_label_type::top);
    auto& label_values_bottom_rotated = layer_rotated.get_label_values(find_neighbours_label_type::bottom);

    std::vector<std::size_t> top_history;
    std::vector<std::size_t> bottom_history;
    top_history.reserve(result.size());
    bottom_history.reserve(result.size());
    result.map_ids();

    for (std::size_t idx = 0; idx < result.size(); ++idx) {
        top_history.clear();
        bottom_history.clear();
        if ((result.get_label_value(mark_areas_label_type::first_circuits_layer_top, result[idx]) != unassigned) &&
              (result.get_label_value(mark_areas_label_type::first_circuits_layer_bottom, result[idx]) != unassigned)) {
            continue;
        }

        bool is_vertical = layer[idx].is_vertical();
        if (is_vertical) {
            bypassNeighbours(label_values_top_rotated, top_history, circuit_layer_id, result, idx, direction::top);
            bypassNeighbours(label_values_bottom_rotated, bottom_history, circuit_layer_id, result, idx, direction::bottom);
        } else {
            bypassNeighbours(label_values_top, top_history, circuit_layer_id, result, idx, direction::top);
            bypassNeighbours(label_values_bottom, bottom_history, circuit_layer_id, result, idx, direction::bottom);
        }
    }

    return result;
}
} // namespace gkernel
