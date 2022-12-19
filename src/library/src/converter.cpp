#include "gkernel/converter.hpp"
#include <map>
#include <array>

namespace gkernel {

    SegmentsLayer Converter::convertToSegmentsLayer(const SegmentsSet& orig_segments, const std::vector<std::tuple<Segment, segment_id, segment_id>>& intersections) {
        auto calc_num_additional_seg_for_point_intersect = [](const Segment& first, const Segment& second, const Point& intersection) -> size_t {
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

        auto select_intersecting_segment = [](const std::vector<Segment>& segments, const Point& intersection) -> const Segment* {
            for (const auto& segment : segments) {
                if ((intersection.x() > segment.min().x() && intersection.x() < segment.max().x()) ||
                    (intersection.y() > segment.min().y() && intersection.y() < segment.max().y())) {
                    return &segment;
                }
            }
            return nullptr;
        };

        auto calc_num_additional_segments = [&calc_num_additional_seg_for_point_intersect]
                                            (const Segment& first, const Segment& second, const Segment& intersection) -> int8_t {
            if (first == second) {
                return -1;
            }
            if (intersection.is_point()) {
                return calc_num_additional_seg_for_point_intersect(first, second, intersection.start());
            }
            if ((first == intersection || second == intersection) &&
                (first.min() == second.min() || first.max() == second.max())) {
                return 0;
            }
            return 1;
        };

        size_t new_segments_count = orig_segments.size();

        for (const auto& intersect_info : intersections) {
            const auto& intersection = std::get<0>(intersect_info);
            const auto& first_seg = orig_segments[std::get<1>(intersect_info)];
            const auto& second_seg = orig_segments[std::get<2>(intersect_info)];

            new_segments_count += calc_num_additional_segments(first_seg, second_seg, intersection);
        }

        std::map<segment_id, std::vector<Segment>> divided_segments;

        for (const auto& intersect_info : intersections) {
            const Segment& intersection = std::get<0>(intersect_info);
            std::array<segment_id, 2> segment_ids = { std::get<1>(intersect_info), std::get<2>(intersect_info) };
            std::array<Segment, 2> segments = { orig_segments[segment_ids.front()], orig_segments[segment_ids.back()] };

            if (intersection.is_point()) {
                Point intersection_point = intersection.start();

                if (calc_num_additional_seg_for_point_intersect(segments.front(), segments.back(), intersection_point) == 0) {
                    continue;
                }

                for (size_t i : {0, 1}) {
                    if (divided_segments.find(segment_ids[i]) != divided_segments.end()) {
                        const std::vector<Segment>& divided_segment = divided_segments.at(segment_ids[i]);
                        const Segment* updated_segment = select_intersecting_segment(divided_segment, intersection_point);
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
            } else {
                // Initial version
                if (segments[0] == segments[1]) {
                    divided_segments[segment_ids[0]] = {};
                    continue;
                }

                if (segments[0] == intersection || segments[1] == intersection) {
                    size_t external_segment_idx = segments[0] == intersection ? 1 : 0;

                    Segment first_part(segments[external_segment_idx].min(), intersection.min());
                    Segment second_part(intersection.max(), segments[external_segment_idx].max());

                    auto& div_segm = divided_segments[segment_ids[external_segment_idx]];
                    if (!first_part.is_point()) {
                        div_segm.emplace_back(first_part);
                    }
                    if (!second_part.is_point()) {
                        div_segm.emplace_back(second_part);
                    }
                } else {
                    size_t left_segment_idx = segments[0].min() < segments[1].min() ? 0 : 1; //mb additional checks needed
                    size_t right_segment_idx = !left_segment_idx;

                    Segment first_part(segments[left_segment_idx].min(), intersection.min());
                    Segment second_part(intersection.min(), intersection.max());
                    Segment third_part(intersection.max(), segments[right_segment_idx].max());

                    divided_segments[segment_ids[left_segment_idx]] = { first_part, second_part };
                    divided_segments[segment_ids[right_segment_idx]] = { third_part };
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

        for (auto& labels_value : labels_values) {
            for (size_t label_id = 0; label_id < label_types.size(); ++label_id) {
                labels_values[label_id].resize(final_size);
                result.set_label_values(label_types[label_id], labels_values[label_id]);
            }
        }

        return static_cast<SegmentsLayer>(result);
    }


    SegmentsLayer Converter::convertToSegmentsLayer(const SegmentsSet& segments) {
        //Call sweeping_straight alg

        std::vector<std::tuple<Segment, segment_id, segment_id>> intersections;
        return convertToSegmentsLayer(segments, intersections);
    }

} // namespace gkernel
