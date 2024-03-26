#include "gkernel/intersection.hpp"
#include "gkernel/rbtree.hpp"

namespace gkernel {

inline double find_k(const Segment& segment) {
    return (segment.max().y() - segment.min().y()) / (segment.max().x() - segment.min().x());
}

inline double find_m(double k, const Segment& segment) {
    return segment.min().y() - k * segment.min().x();
}

inline bool on_one_line(const Segment& first, const Segment& second) {
    double k_first = find_k(first);
    double k_second = find_k(second);
    double m_first = find_m(k_first, first);
    double m_second = find_m(k_second, second);

    if (std::abs(k_first - k_second) < EPS) {
        return std::abs(m_first - m_second) < EPS;
    }

    return false;
}

// old implementation, detects overlapping segments
inline int get_area(const Point& a, const Point& b, const Point& c) {
    data_type area = (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
    return area == 0 ? 0 : area > 0 ? 1 : -1;
}

inline Intersection::segments_relation intersect_or_overlap(const Segment& first, const Segment& second) {
    bool is_on_one_line = on_one_line(first, second);

    if (is_on_one_line) {
        if (first.is_vertical() && second.is_vertical()) {
            if (second.min().y() <= first.min().y() && first.min().y() < second.max().y()) {
                return Intersection::segments_relation::overlap;
            }
            if (first.min().y() <= second.min().y() && second.min().y() < first.max().y()) {
                return Intersection::segments_relation::overlap;
            }
        } else {
            if (second.min().x() <= first.min().x() && first.min().x() < second.max().x()) {
                return Intersection::segments_relation::overlap;
            }
            if (first.min().x() <= second.min().x() && second.min().x() < first.max().x()) {
                return Intersection::segments_relation::overlap;
            }
        }
        return Intersection::segments_relation::none;
    }

    if (first.min() == second.min() || first.min() == second.max() || first.max() == second.min() || first.max() == second.max()) {
        return Intersection::segments_relation::none;
    }

    bool is_intersect = get_area(first.min(), first.max(), second.min()) * get_area(first.min(), first.max(), second.max()) <= 0 &&
                    get_area(second.min(), second.max(), first.min()) * get_area(second.min(), second.max(), first.max()) <= 0;

    return is_intersect ? Intersection::segments_relation::intersect : Intersection::segments_relation::none;
}

// inline int orientation(const Point& first, const Point& second, const Point& third) {
//     double val = (second.y() - first.y()) * (third.x() - second.x()) -
//               (second.x() - first.x()) * (third.y() - second.y());
//     if (val == 0) return 0;
//     return (val > 0)? 1: 2;
// }

// inline bool intersect_or_overlap(const Segment& first, const Segment& second) {
//     if (first.min() == second.min() || first.min() == second.end() || first.end() == second.min() || first.end() == second.end()) {
//         return false;
//     }
//     double o1 = orientation(first.min(), first.end(), second.min());
//     double o2 = orientation(first.min(), first.end(), second.end());
//     double o3 = orientation(second.min(), second.end(), first.min());
//     double o4 = orientation(second.min(), second.end(), first.end());
//     return o1 != o2 && o3 != o4;
// }

// intersect two segments
Point Intersection::intersectSegments(const Segment& first, const Segment& second) {
    data_type a1 = first.max().y() - first.min().y();
    data_type b1 = first.min().x() - first.max().x();
    data_type c1 = first.min().y() * first.max().x() -
                            first.min().x() * first.max().y();

    data_type a2 = second.max().y() - second.min().y();
    data_type b2 = second.min().x() - second.max().x();
    data_type c2 = second.min().y() * second.max().x() -
                            second.min().x() * second.max().y();
    if ((a1 * b2 - a2 * b1) != 0) {
        data_type x = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
        data_type y = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);
        return Point({x, y});
    }
    else {
        if (first.min() == second.min())
            return Point({first.min().x(), first.min().y()});
        else if (first.max() == second.max())
            return Point({first.max().x(), first.max().y()});
        else if (first.min() == second.max())
            return Point({first.min().x(), first.min().y()});
        else if (first.max() == second.min())
            return Point({first.max().x(), first.max().y()});
    }
    throw_exception("Segments do not intersect");
    return Point();
}

std::pair<Point, Point> Intersection::overlapSegments(const Segment& first, const Segment& second) {
    Point first_overlap_point;
    Point second_overlap_point;
    if (second.min().x() <= first.min().x() && first.min().x() <= second.max().x()) {
        first_overlap_point = first.min();
    }
    if (second.min().x() <= first.max().x() && first.max().x() <= second.max().x()) {
        second_overlap_point = first.max();
    }
    if (first.min().x() <= second.min().x() && second.min().x() <= first.max().x()) {
        first_overlap_point = second.min();
    }
    if (first.min().x() <= second.max().x() && second.max().x() <= first.max().x()) {
        second_overlap_point = second.max();
    }
    return std::make_pair(first_overlap_point, second_overlap_point);
}

std::pair<Point, Point> Intersection::overlapSegmentsVertical(const Segment& first, const Segment& second) {
    Point first_overlap_point;
    Point second_overlap_point;
    if (second.min().y() <= first.min().y() && first.min().y() <= second.max().y()) {
        first_overlap_point = first.min();
    }
    if (second.min().y() <= first.max().y() && first.max().y() <= second.max().y()) {
        second_overlap_point = first.max();
    }
    if (first.min().y() <= second.min().y() && second.min().y() <= first.max().y()) {
        first_overlap_point = second.min();
    }
    if (first.min().y() <= second.max().y() && second.max().y() <= first.max().y()) {
        second_overlap_point = second.max();
    }
    return std::make_pair(first_overlap_point, second_overlap_point);
}

std::vector<IntersectionSegment> Intersection::intersectSetSegments(const SegmentsSet& segments) {
    std::vector<IntersectionSegment> result;

    if (segments.size() == 0) {
        return result;
    }

    double x_sweeping_line = 0;

    auto compare_segments = [&x_sweeping_line](const Segment* first, const Segment* second) -> bool {
        double local_eps = -EPS;
        if (first->min().x() == x_sweeping_line || second->min().x() == x_sweeping_line) {
            local_eps = EPS;
        }
        double y1 = get_sweeping_line_y(*first, x_sweeping_line, local_eps);
        double y2 = get_sweeping_line_y(*second, x_sweeping_line, local_eps);
        return std::tie(y1, first->id) < std::tie(y2, second->id);
    };

    using tree_type = RBTree<const Segment*, decltype(compare_segments)>;
    tree_type active_segments(compare_segments);

    std::set<Event> events;

	for (std::size_t idx = 0; idx < segments.size(); ++idx) {
        Segment& segment = const_cast<Segment&>(segments[idx]);
        if (!segment.is_vertical()) {
            events.insert({ segment, segment.min().x(), event_status::start });
            events.insert({ segment, segment.max().x(), event_status::end });
        }
        else {
            events.insert({ segment, segment.start().x(), event_status::vertical });
        }
	}

    std::vector<Segment*> temp_new_order;
    std::vector<Segment**> temp_prev_order;
    temp_new_order.reserve(segments.size());
    temp_prev_order.reserve(segments.size());
    double prev_reorder_x_sweeping_line = -1;
    while (events.begin() != events.end()) {
        auto event = *events.begin();
        if (event.status == event_status::intersection_right && prev_reorder_x_sweeping_line != event.x) {
            auto event_it = events.begin();
            temp_new_order.clear();
            temp_prev_order.clear();
            while (event_it != events.end() && event_it->status == event_status::intersection_right && event_it->x == event.x) {
                if (std::abs(event_it->segment->max().x() - event.x) < 5 * EPS) {
                    ++event_it;
                    continue;
                }
                auto insert_result = active_segments.insert(event_it->segment);
                #if GKERNEL_DEBUG
                if (insert_result.second) {
                    throw_exception("error: insertion in a place where it is already exist");
                }
                #endif
                Segment** segment_ptr = const_cast<Segment**>(&(*insert_result.first));
                temp_new_order.push_back(const_cast<Segment*>(event_it->segment));
                temp_prev_order.push_back(segment_ptr);
                ++event_it;
            }
            std::sort(temp_prev_order.begin(), temp_prev_order.end(), [&compare_segments](auto& first_segment, auto& second_segment) {
                return compare_segments(*first_segment, *second_segment);
            });
            x_sweeping_line = event.x;
            std::sort(temp_new_order.begin(), temp_new_order.end(), [&compare_segments](auto& first_segment, auto& second_segment) {
                return compare_segments(first_segment, second_segment);
            });
            for (std::size_t idx = 0; idx < temp_new_order.size(); ++idx) {
                (*temp_prev_order[idx]) = temp_new_order[idx];
            }
            prev_reorder_x_sweeping_line = event.x;
        } else {
            x_sweeping_line = event.x;
        }
        auto event_it = events.begin();

        if (event.status == event_status::vertical) {
            auto lower_bound = Segment(Point{event.segment->min().x() - 10 * EPS, event.segment->min().y() - 10 * EPS},
                                       Point{event.segment->min().x() + 10 * EPS, event.segment->min().y() - 10 * EPS});
            auto current_segment = active_segments.find_prev(&lower_bound).first;
            if (current_segment == active_segments.end()) {
                events.erase(events.begin());
                continue;
            }
            double current_y = get_sweeping_line_y(**current_segment, x_sweeping_line);
            while (current_y <= event.segment->max().y()) {
                auto seg_rel_status = intersect_or_overlap(*event.segment, **current_segment);
                if (seg_rel_status == Intersection::segments_relation::intersect) {
                    auto intersection = intersectSegments(*event.segment, **current_segment);
                    result.emplace_back(intersection, event.segment->id, (**current_segment).id);
                }
                ++current_segment;
                if (current_segment == active_segments.end()) {
                    break;
                }
                current_y = get_sweeping_line_y(**current_segment, x_sweeping_line);
            }
            events.erase(events.begin());
            continue;
        }

        #if GKERNEL_DEBUG
        decltype(active_segments.insert(event.segment)) insert_result;
        if (event.status == event_status::start) {
            insert_result = active_segments.insert(event.segment);
            if (!insert_result.second) {
                throw_exception("error: not inserted, but it should be");
            }
        } else {
            insert_result = active_segments.insert(event.segment);
            if (insert_result.second) {
                throw_exception("error: insertion in a place where it is already exist");
            }
        }
        #else
        auto insert_result = active_segments.insert(event.segment);
        #endif

        auto prev_segment = insert_result.first;
        if (prev_segment != active_segments.begin()) {
            --prev_segment;
            auto seg_rel_status = intersect_or_overlap(*event.segment, **prev_segment);
            if (seg_rel_status == Intersection::segments_relation::intersect) {
                auto intersection = intersectSegments(*event.segment, **prev_segment);
                result.emplace_back(intersection, event.segment->id, (*prev_segment)->id);
                if (intersection.x() >= event.x) {
                    double offset = 0;
                    if ((intersection.x() - event.x) > 3 * EPS || event.status != event_status::intersection_right) {
                        offset = 3 * EPS;
                    }
                    if (std::abs(event.segment->max().x() - intersection.x()) > offset) {
                        events.insert({*event.segment, intersection.x() + offset, event_status::intersection_right});
                    }
                    if (std::abs((*prev_segment)->max().x() - intersection.x()) > offset) {
                        events.insert({**prev_segment, intersection.x() + offset, event_status::intersection_right});
                    }
                }
            }
            if (seg_rel_status == Intersection::segments_relation::overlap) {
                auto overlap = overlapSegments(*event.segment, **prev_segment);
                result.emplace_back(overlap.first, overlap.second, event.segment->id, (*prev_segment)->id);
            }
        }

        auto next_segment = insert_result.first;
        ++next_segment;
        if (next_segment != active_segments.end()) {
            auto seg_rel_status = intersect_or_overlap(*event.segment, **next_segment);
            if (seg_rel_status == Intersection::segments_relation::intersect) {
                auto intersection = intersectSegments(*event.segment, **next_segment);
                result.emplace_back(intersection, event.segment->id, (*next_segment)->id);
                if (intersection.x() >= event.x) {
                    double offset = 0;
                    if ((intersection.x() - event.x) > 3 * EPS || event.status != event_status::intersection_right) {
                        offset = 3 * EPS;
                    }
                    if (std::abs(event.segment->max().x() - intersection.x()) > offset) {
                        events.insert({*event.segment, intersection.x() + offset, event_status::intersection_right});
                    }
                    if (std::abs((*next_segment)->max().x() - intersection.x()) > offset) {
                        events.insert({**next_segment, intersection.x() + offset, event_status::intersection_right});
                    }
                }
            }
            if (seg_rel_status == Intersection::segments_relation::overlap) {
                auto overlap = overlapSegments(*event.segment, **next_segment);
                result.emplace_back(overlap.first, overlap.second, event.segment->id, (*next_segment)->id);
            }
        }

        if (event.status == event_status::end) {
            if (next_segment != active_segments.end()) {
                auto seg_rel_status = intersect_or_overlap(**prev_segment, **next_segment);
                if (seg_rel_status == Intersection::segments_relation::intersect) {
                    auto intersection = intersectSegments(**prev_segment, **next_segment);
                    result.emplace_back(intersection, (*prev_segment)->id, (*next_segment)->id);
                    if (intersection.x() >= event.x) {
                        double offset = 0;
                        if ((intersection.x() - event.x) > 3 * EPS || event.status != event_status::intersection_right) {
                            offset = 3 * EPS;
                        }
                        if (std::abs((*prev_segment)->max().x() - intersection.x()) > offset) {
                            events.insert({**prev_segment, intersection.x() + offset, event_status::intersection_right});
                        }
                        if (std::abs((*next_segment)->max().x() - intersection.x()) > offset) {
                            events.insert({**next_segment, intersection.x() + offset, event_status::intersection_right});
                        }
                    }
                }
                if (seg_rel_status == Intersection::segments_relation::overlap) {
                    auto overlap = overlapSegments(**prev_segment, **next_segment);
                    result.emplace_back(overlap.first, overlap.second, (*prev_segment)->id, (*next_segment)->id);
                }
            }
            auto count = active_segments.erase(event.segment);
            #if GKERNEL_DEBUG
            if (count == 0) {
                throw_exception("error: not erased, but it should be");
            }
            if (count > 1) {
                throw_exception("error: erased more than one");
            }
            #endif
        }
        events.erase(event_it);
    }

    return result;
}

} // namespace gkernel
