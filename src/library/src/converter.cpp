#include "gkernel/converter.hpp"
#include "gkernel/area_analyzer.hpp"
#include "gkernel/intersection.hpp"
#include <map>
#include <array>

namespace gkernel {

const Segment* selectIntersectingSegment(const std::vector<Segment>& segments, const Point& intersection) {
    for (const auto& segment : segments) {
        if ((intersection.x() > segment.min().x() && intersection.x() < segment.max().x()) ||
            (intersection.y() > segment.min().y() && intersection.y() < segment.max().y())) {
            return &segment;
        }
    }
    return nullptr;
}

void splitSegments(std::map<segment_id, std::vector<Segment>>& divided_segments, std::array<segment_id, 2>& segment_ids, std::array<Segment, 2>& segments, Point& intersection_point, std::initializer_list<std::size_t> indices = {0, 1}) {
    for (size_t i : indices) {
        if (divided_segments.find(segment_ids[i]) != divided_segments.end()) {
            const std::vector<Segment>& divided_segment = divided_segments.at(segment_ids[i]);
            const Segment* updated_segment = selectIntersectingSegment(divided_segment, intersection_point);
            if (!updated_segment) {
                continue;
            }
            Segment first_part(updated_segment->min(), intersection_point);
            Segment second_part(intersection_point, updated_segment->max());

            std::vector<Segment> updated_division;
            updated_division.reserve(divided_segment.size() + 1);
            for (const auto& seg : divided_segment) {
                if (seg != *updated_segment) {
                    updated_division.emplace_back(seg);
                }
            }
            updated_division.emplace_back(first_part);
            updated_division.emplace_back(second_part);
            divided_segments[segment_ids[i]] = updated_division;
        } else {
            Segment first_part(segments[i].min(), intersection_point);
            Segment second_part(intersection_point, segments[i].max());

            auto& div_segm = divided_segments[segment_ids[i]];

            if (!first_part.is_point()) {
                div_segm.emplace_back(first_part);
            }
            if (!second_part.is_point()) {
                div_segm.emplace_back(second_part);
            }
        }
    }
}

SegmentsLayer Converter::_convertToSegmentsLayer(const SegmentsSet& orig_segments, const std::vector<IntersectionSegment>& intersections) {
    auto calc_num_additional_seg_for_point_intersect = [](const Segment& first, const Segment& second, const Point& intersection) -> int8_t {
        size_t count_intersect_with_endpoints = static_cast<size_t>(first.start()  == intersection) +
                                                static_cast<size_t>(first.end()    == intersection) +
                                                static_cast<size_t>(second.start() == intersection) +
                                                static_cast<size_t>(second.end()   == intersection);
        switch (count_intersect_with_endpoints) {
            case 0: return 2;
            case 1: return 1;
            case 2: return 0;
            default: throw std::runtime_error("Intersection between point and segment is not supported.");
        }
    };

    auto calc_num_additional_segments = [&calc_num_additional_seg_for_point_intersect]
                                        (const Segment& first, const Segment& second, const Segment& intersection) -> int8_t {
        if (first == second) {
            return -1;
        }
        if (intersection.is_point()) {
            return calc_num_additional_seg_for_point_intersect(first, second, intersection.start());
        } else {
            return 4;
        }
        return 1;
    };

    int64_t new_segments_count = orig_segments.size();

    for (const auto& intersect_info : intersections) {
        const auto& intersection = Segment(intersect_info.first_point(), intersect_info.second_point());
        const auto& first_seg = orig_segments[intersect_info.first_id()];
        const auto& second_seg = orig_segments[intersect_info.second_id()];

        new_segments_count += calc_num_additional_segments(first_seg, second_seg, intersection);
    }

    new_segments_count = std::max(new_segments_count, static_cast<int64_t>(orig_segments.size()));

    std::map<segment_id, std::vector<Segment>> divided_segments;

    for (const auto& intersect_info : intersections) {
        const Segment& intersection = Segment(intersect_info.first_point(), intersect_info.second_point());
        std::array<segment_id, 2> segment_ids = { intersect_info.first_id(), intersect_info.second_id() };
        std::array<Segment, 2> segments = { orig_segments[segment_ids.front()], orig_segments[segment_ids.back()] };

        if (intersection.is_point()) {
            Point intersection_point = intersection.start();

            if (calc_num_additional_seg_for_point_intersect(segments.front(), segments.back(), intersection_point) == 0) {
                continue;
            }

            splitSegments(divided_segments, segment_ids, segments, intersection_point);
        } else {
            if (segments[0] == intersection || segments[1] == intersection) {
                size_t external_segment_idx = segments[0] == intersection ? 1 : 0;
                Point intersection_point_min = intersection.min();
                Point intersection_point_max = intersection.max();

                if (intersection_point_min != segments[external_segment_idx].min()) {
                    splitSegments(divided_segments, segment_ids, segments, intersection_point_min);
                }

                if (intersection_point_max != segments[external_segment_idx].min()) {
                    splitSegments(divided_segments, segment_ids, segments, intersection_point_max);
                }
            } else {
                size_t left_segment_idx = segments[0].min() < segments[1].min() ? 0 : 1; //mb additional checks needed
                size_t right_segment_idx = !left_segment_idx;

                Point intersection_point_min = intersection.min();
                Point intersection_point_max = intersection.max();

                splitSegments(divided_segments, segment_ids, segments, intersection_point_min, {left_segment_idx});
                splitSegments(divided_segments, segment_ids, segments, intersection_point_max, {right_segment_idx});
            }
        }
    }

    std::vector<Segment> init_layer(new_segments_count);
    segment_id final_size = new_segments_count;

    const auto& label_types = orig_segments.get_label_types();
    std::vector<std::vector<label_data_type>> labels_values(label_types.size(), std::vector<label_data_type>(new_segments_count));

    auto fill_labels_values_segment_info = [&](segment_id idx, const Segment& segment) {
        for (size_t label_id = 0; label_id < label_types.size(); ++label_id) {
            labels_values[label_id][idx] = orig_segments.get_label_value(label_types[label_id], segment);
        }
    };

    for (segment_id i = 0, out_idx = 0; i < orig_segments.size(); ++i) {
        if (divided_segments.find(i) != divided_segments.end()) {
            const std::vector<Segment>& divided_segment = divided_segments.at(i);
            for (const auto& segment : divided_segment) {
                init_layer[out_idx] = segment;
                init_layer[out_idx].id = out_idx;

                fill_labels_values_segment_info(out_idx, orig_segments[i]);

                final_size = ++out_idx;
            }
        } else {
            init_layer[out_idx] = orig_segments[i];
            init_layer[out_idx].id = out_idx;

            fill_labels_values_segment_info(out_idx, orig_segments[i]);

            final_size = ++out_idx;
        }
    }
    init_layer.resize(final_size);

    SegmentsSet result(init_layer);

    result.set_labels_types(label_types);

    for (size_t label_id = 0; label_id < label_types.size(); ++label_id) {
        labels_values[label_id].resize(final_size);
        result.set_label_values(label_types[label_id], labels_values[label_id]);
    }

    // incorrect ordering of the labels after sort

    // TODO: rework labels reordering, this is temporary solution. Works only for 0 label
    if (result._labels_data.size() == 0) {
        return static_cast<SegmentsLayer>(result);
    }

    std::vector<std::pair<gkernel::Segment, std::size_t>> pairs;
    for (std::size_t idx = 0; idx < result._segments.size(); ++idx) {
        pairs.emplace_back(result._segments[idx], idx);
    }

    std::sort(pairs.begin(), pairs.end(), [](auto& lhs, auto& rhs) {
        if (lhs.first.min().x() != rhs.first.min().x()) {
            return lhs.first.min().x() < rhs.first.min().x();
        }
        if (lhs.first.min().y() != rhs.first.min().y()) {
            return lhs.first.min().y() < rhs.first.min().y();
        }
        if (lhs.first.max().x() != rhs.first.max().x()) {
            return lhs.first.max().x() < rhs.first.max().x();
        }
        if (lhs.first.max().y() != rhs.first.max().y()) {
            return lhs.first.max().y() < rhs.first.max().y();
        }
        return lhs.first.get_id() < rhs.first.get_id();
    });

    auto old_labels_data = result._labels_data;

    for (std::size_t idx = 0; idx < pairs.size(); ++idx) {
        result._segments[idx] = pairs[idx].first;
        for (std::size_t label_id = 0; label_id < result._labels_data.size(); ++label_id) {
            result._labels_data[label_id][idx] = old_labels_data[label_id][pairs[idx].second];
        }
    }

    std::vector<std::size_t> to_remove;
    if (label_types.size() > 0) {
        for (std::size_t idx = 0; idx < result.size() - 1; ++idx) {
            if (result[idx] == result[idx + 1]) {
                result._labels_data[0][idx] = 2;
                to_remove.push_back(idx + 1);
            }
        }
    }

    for (int64_t idx = to_remove.size() - 1; idx >= 0; --idx) {
        std::size_t idx_to_remove = to_remove[idx];
        result._segments[idx_to_remove] = result._segments.back();
        result._segments.pop_back();
        for (std::size_t idx2 = 0; idx2 < result._labels_data.size(); ++idx2) {
            result._labels_data[idx2][idx_to_remove] = result._labels_data[idx2].back();
            result._labels_data[idx2].pop_back();
        }
    }

    for (std::size_t idx = 0; idx < result._segments.size(); ++idx) {
        result._segments[idx].id = idx;
    }

    return static_cast<SegmentsLayer>(result);
}

SegmentsSet Converter::mergeCircuitsLayers(const CircuitsLayer& first_layer, const CircuitsLayer& second_layer) {
    std::vector<Segment> temp_result;
    temp_result.reserve(first_layer._segments.size() + second_layer._segments.size());
    std::copy(first_layer._segments.begin(), first_layer._segments.end(), std::back_inserter(temp_result));
    std::copy(second_layer._segments.begin(), second_layer._segments.end(), std::back_inserter(temp_result));
    SegmentsSet result(temp_result);
    result.set_labels_types({ 0 });

    for (std::size_t idx = 0; idx < first_layer._segments.size(); ++idx) {
        result.set_label_value(0, result[idx], 0);
    }

    for (std::size_t idx = first_layer._segments.size(); idx < temp_result.size(); ++idx) {
        result.set_label_value(0, result[idx], 1);
    }

    return result;
}

SegmentsLayer Converter::convertToSegmentsLayer(const SegmentsSet& segments) {
    auto intersections = Intersection::intersectSetSegments(segments);
    return convertToSegmentsLayer(segments, intersections);
}

} // namespace gkernel
