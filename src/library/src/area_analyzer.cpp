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
                ++current_event;
                continue;
            }

            ++current_event;
        }

        for (auto current_segment : active_segments_new) {
            result.set_label_value(find_neighbours_label_type::top, **current_segment, unassigned);
            result.set_label_value(find_neighbours_label_type::bottom, **current_segment, unassigned);

            auto next_segment = current_segment;
            ++next_segment;
            if (next_segment != active_segments.end()) {
                if (!rotated) {
                    result.set_label_value(find_neighbours_label_type::top, **current_segment, (**next_segment).id);
                } else {
                    result.set_label_value(find_neighbours_label_type::bottom, **current_segment, (**next_segment).id);
                }
            }

            auto prev_segment = current_segment;
            if (prev_segment != active_segments.begin()) {
                --prev_segment;
                if (!rotated) {
                    result.set_label_value(find_neighbours_label_type::bottom, **current_segment, (**prev_segment).id);
                } else {
                    result.set_label_value(find_neighbours_label_type::top, **current_segment, (**prev_segment).id);
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

    AreaAnalyzer::internalFindSegmentsNeighbours(layer, result_rotated, true);

    return std::make_pair(result, result_rotated);
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


    result.set_labels_types({ mark_areas_label_type::first_circuits_layer_top, mark_areas_label_type::second_circuits_layer_top,
                              mark_areas_label_type::first_circuits_layer_bottom, mark_areas_label_type::second_circuits_layer_bottom });

    for (std::size_t idx = 0; idx < result.size(); ++idx) {
        result.set_label_value(mark_areas_label_type::first_circuits_layer_top, result[idx], unassigned);
        result.set_label_value(mark_areas_label_type::first_circuits_layer_bottom, result[idx], unassigned);
    }

    auto label_values_id = layer.get_label_values(find_neighbours_label_type::circuits_layer_id);
    auto label_values_top = layer.get_label_values(find_neighbours_label_type::top);
    auto label_values_bottom = layer.get_label_values(find_neighbours_label_type::bottom);

    auto label_values_top_rotated = layer_rotated.get_label_values(find_neighbours_label_type::top);
    auto label_values_bottom_rotated = layer_rotated.get_label_values(find_neighbours_label_type::bottom);

    std::vector<std::size_t> top_history;
    std::vector<std::size_t> bottom_history;
    top_history.reserve(result.size());
    bottom_history.reserve(result.size());

    for (std::size_t idx = 0; idx < result.size(); ++idx) {
        if ((result.get_label_value(mark_areas_label_type::first_circuits_layer_top, result[idx]) != unassigned) &&
              (result.get_label_value(mark_areas_label_type::first_circuits_layer_bottom, result[idx]) != unassigned)) {
            continue;
        }

        bool is_vertical = layer[idx].min().x() == layer[idx].max().x();
        if (is_vertical) {
            label_data_type top = label_values_top_rotated[layer[idx].id];
            top_history.push_back(idx);

            while (top != unassigned && top != unchecked_segment) {
                top_history.push_back(top);
                top = label_values_top_rotated[layer[top].id];
            }
        } else {
            label_data_type top = label_values_top[layer[idx].id];
            top_history.push_back(idx);

            while (top != unassigned && top != unchecked_segment) {
                top_history.push_back(top);
                top = label_values_top[layer[top].id];
            }
        }

        if (layer[idx].min().x() == layer[idx].max().x()) {
            label_data_type bottom = label_values_bottom_rotated[layer[idx].id];
            bottom_history.push_back(idx);

            while (bottom != unassigned && bottom != unchecked_segment) {
                bottom_history.push_back(bottom);
                bottom = label_values_bottom_rotated[layer[bottom].id];
            }
        } else {
            label_data_type bottom = label_values_bottom[layer[idx].id];
            bottom_history.push_back(idx);

            while (bottom != unassigned && bottom != unchecked_segment) {
                bottom_history.push_back(bottom);
                bottom = label_values_bottom[layer[bottom].id];
            }
        }

        if ((result[top_history.back()].start().x() == result[top_history.back()].end().x()) ==
              is_vertical) {
            result.set_label_value(mark_areas_label_type::first_circuits_layer_top, result[top_history.back()], false);
            result.set_label_value(mark_areas_label_type::second_circuits_layer_top, result[top_history.back()], false);
        }

        if (top_history.size() > 1) {
            bool first_circuits_layer_top = false;
            bool second_circuits_layer_top = false;
            for (auto top_idx_iter = top_history.rbegin() + 1; top_idx_iter != top_history.rend(); ++top_idx_iter) {
                if (label_values_id[layer[*(top_idx_iter - 1)].id] == 0) {
                    first_circuits_layer_top ^= true;
                } else {
                    second_circuits_layer_top ^= true;
                }
                if ((result[*top_idx_iter].start().x() == result[*top_idx_iter].end().x()) ==
                      is_vertical) {
                    result.set_label_value(mark_areas_label_type::first_circuits_layer_top, result[*top_idx_iter], first_circuits_layer_top);
                    result.set_label_value(mark_areas_label_type::second_circuits_layer_top, result[*top_idx_iter], second_circuits_layer_top);
                }
            }
        }

        if ((result[bottom_history.back()].start().x() == result[bottom_history.back()].end().x()) ==
              is_vertical) {
            result.set_label_value(mark_areas_label_type::first_circuits_layer_bottom, result[bottom_history.back()], false);
            result.set_label_value(mark_areas_label_type::second_circuits_layer_bottom, result[bottom_history.back()], false);
        }
        if (bottom_history.size() > 1) {
            bool first_circuits_layer_bottom = false;
            bool second_circuits_layer_bottom = false;
            for (auto bottom_idx_iter = bottom_history.rbegin() + 1; bottom_idx_iter != bottom_history.rend(); ++bottom_idx_iter) {
                if (label_values_id[layer[*(bottom_idx_iter - 1)].id] == 0) {
                    first_circuits_layer_bottom ^= true;
                } else {
                    second_circuits_layer_bottom ^= true;
                }
                if ((result[*bottom_idx_iter].start().x() == result[*bottom_idx_iter].end().x()) ==
                      is_vertical) {
                    result.set_label_value(mark_areas_label_type::first_circuits_layer_bottom, result[*bottom_idx_iter], first_circuits_layer_bottom);
                    result.set_label_value(mark_areas_label_type::second_circuits_layer_bottom, result[*bottom_idx_iter], second_circuits_layer_bottom);
                }
            }
        }

        top_history.clear();
        bottom_history.clear();
    }

    return result;
}
} // namespace gkernel
