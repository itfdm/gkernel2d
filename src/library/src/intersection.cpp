#include "gkernel/intersection.hpp"
#include "gkernel/rbtree.hpp"

namespace gkernel {


// old implementation, detects overlapping segments
// inline int get_area(const Point& a, const Point& b, const Point& c) {
//     data_type area = (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
//     return area == 0 ? 0 : area > 0 ? 1 : -1;
// }

// inline bool intersect(const Segment& first, const Segment& second) {
//     return get_area(first.start(), first.end(), second.start()) * get_area(first.start(), first.end(), second.end()) <= 0 &&
//            get_area(second.start(), second.end(), first.start()) * get_area(second.start(), second.end(), first.end()) <= 0;
// }

inline int orientation(const Point& first, const Point& second, const Point& third) {
    double val = (second.y() - first.y()) * (third.x() - second.x()) -
              (second.x() - first.x()) * (third.y() - second.y());
    if (val == 0) return 0;
    return (val > 0)? 1: 2;
}

inline bool intersect(const Segment& first, const Segment& second) {
    double o1 = orientation(first.start(), first.end(), second.start());
    double o2 = orientation(first.start(), first.end(), second.end());
    double o3 = orientation(second.start(), second.end(), first.start());
    double o4 = orientation(second.start(), second.end(), first.end());
    if (o1 != o2 && o3 != o4) {
        return true;
    }

    return false;
}

// intersect two segments
Point Intersection::intersectSegments(const Segment& first, const Segment& second) {
    data_type a1 = first.end().y() - first.start().y();
    data_type b1 = first.start().x() - first.end().x();
    data_type c1 = first.start().y() * first.end().x() -
                            first.start().x() * first.end().y();

    data_type a2 = second.end().y() - second.start().y();
    data_type b2 = second.start().x() - second.end().x();
    data_type c2 = second.start().y() * second.end().x() -
                            second.start().x() * second.end().y();
    if ((a1 * b2 - a2 * b1) != 0) {
        data_type x = (b1 * c2 - b2 * c1) / (a1 * b2 - a2 * b1);
        data_type y = (a2 * c1 - a1 * c2) / (a1 * b2 - a2 * b1);
        return Point({x, y});
    }
    else {
        if (first.start() == second.start())
            return Point({first.start().x(), first.start().y()});
        else if (first.end() == second.end())
            return Point({first.end().x(), first.end().y()});
        else if (first.start() == second.end())
            return Point({first.start().x(), first.start().y()});
        else if (first.end() == second.start())
            return Point({first.end().x(), first.end().y()});
    }
    throw_exception("Segments do not intersect");
    return Point();
}

std::vector<IntersectionPoint> Intersection::intersectSetSegments(const SegmentsSet& segments) {
    std::vector<IntersectionPoint> result;

    if (segments.size() == 0) {
        return result;
    }

	std::set<Event> events;
    double x_sweeping_line = 0;

    auto compare_segments = [&x_sweeping_line](const Segment* first, const Segment* second) -> bool {
        double eps = 0;
        if (first->max().x() == x_sweeping_line || second->max().x() == x_sweeping_line ||
            (first->min().x() != x_sweeping_line && second->min().x() != x_sweeping_line)) {
            eps = -EPS;
        }
        double y1 = get_sweeping_line_y(*first, x_sweeping_line, eps);
        double y2 = get_sweeping_line_y(*second, x_sweeping_line, eps);
        return std::tie(y1, first->id) < std::tie(y2, second->id);
    };

    using tree_type = RBTree<const Segment*, decltype(compare_segments)>;
    tree_type active_segments(compare_segments);

	for (std::size_t idx = 0; idx < segments.size(); ++idx) {
        Segment& segment = const_cast<Segment&>(segments[idx]);
        if (segment.start().x() != segment.end().x()) {
            events.insert({ segment, std::min(segment.start().x(), segment.end().x()), event_status::start });
            events.insert({ segment, std::max(segment.start().x(), segment.end().x()), event_status::end });
        }
        else {
            events.insert({ segment, segment.start().x(), event_status::vertical });
        }
	}

    std::vector<const Segment*> temp_segments;
    temp_segments.reserve(segments.size());
    double x_sweeping_line_prev_right = -1;
    while (events.size() > 0) {
        auto event = *events.begin();
        temp_segments.clear();
        if (event.status == event_status::intersection_right && x_sweeping_line_prev_right != event.x) {
            auto event_it = events.begin();
            while (event_it != events.end() && event_it->x == event.x && event_it->status == event_status::intersection_right) {
                double y = get_sweeping_line_y(*(*event_it).segment, (*event_it).x);
                auto lower_bound = Segment(Point{(*event_it).x - 2 * EPS, y - EPS}, Point{(*event_it).x, y - EPS});
                auto upper_bound = Segment(Point{(*event_it).x - 2 * EPS, y + EPS}, Point{(*event_it).x, y + EPS});;
                auto start_segment = active_segments.find_prev(&lower_bound).first;
                auto end_segment = active_segments.find_next(&upper_bound).first;
                if (end_segment != active_segments.end()) {
                    ++end_segment;
                }

                while (start_segment != end_segment) {
                    auto segment_to_remove = start_segment;
                    ++start_segment;
                    temp_segments.push_back(*segment_to_remove);
                    auto count = active_segments.erase(*segment_to_remove);
                    #if GKERNEL_DEBUG
                    if (count == 0) {
                        throw_exception("error: not erased, but it should be");
                    }
                    if (count > 1) {
                        throw_exception("error: erased more than one");
                    }
                    #endif
                }

                ++event_it;
            }
            x_sweeping_line = event.x;
            x_sweeping_line_prev_right = x_sweeping_line;
            for (auto& segment : temp_segments) {
                auto insert_result = active_segments.insert(segment);
                #if GKERNEL_DEBUG
                if (!insert_result.second) {
                    throw_exception("error: not inserted, but it should be");
                }
                #endif
            }
        } else {
            x_sweeping_line = event.x;
        }
        auto event_it = events.begin();

        if (event.status == event_status::vertical) {
            auto lower_bound = Segment(Point{event.segment->min().x() - EPS, event.segment->min().y()}, Point{event.segment->min().x() + EPS, event.segment->min().y()});
            auto upper_bound = Segment(Point{event.segment->min().x() - EPS, event.segment->max().y()}, Point{event.segment->min().x() + EPS, event.segment->max().y()});
            auto start_segment = active_segments.find_prev(&lower_bound).first;
            auto end_segment = active_segments.find_next(&upper_bound).first;
            if (end_segment != active_segments.end()) {
                ++end_segment;
            }
            while (start_segment != end_segment) {
                if (intersect(*event.segment, **start_segment)) {
                    auto intersection = intersectSegments(*event.segment, **start_segment);
                    result.emplace_back(intersection, event.segment->id, (**start_segment).id);
                }
                ++start_segment;
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
            if (intersect(*event.segment, **prev_segment)) {
                auto intersection = intersectSegments(*event.segment, **prev_segment);
                result.emplace_back(intersection, event.segment->id, (*prev_segment)->id);
                if ((intersection.x() >= event.x) && ((intersection.x() - event.x) > 5 * EPS || event.status != event_status::intersection_right)) {
                    if (std::abs(event.segment->max().x() - intersection.x()) > EPS) {
                        events.insert({*event.segment, intersection.x() + 3 * EPS, event_status::intersection_right});
                    }
                    if (std::abs((*prev_segment)->max().x() - intersection.x()) > EPS) {
                        events.insert({**prev_segment, intersection.x() + 3 * EPS, event_status::intersection_right});
                    }
                }
            }
        }

        auto next_segment = insert_result.first;
        ++next_segment;
        if (next_segment != active_segments.end()) {
            if (intersect(*event.segment, **next_segment)) {
                auto intersection = intersectSegments(*event.segment, **next_segment);
                result.emplace_back(intersection, event.segment->id, (*next_segment)->id);
                if ((intersection.x() >= event.x) && ((intersection.x() - event.x) > 5 * EPS || event.status != event_status::intersection_right)) {
                    if (std::abs(event.segment->max().x() - intersection.x()) > EPS) {
                        events.insert({*event.segment, intersection.x() + 3 * EPS, event_status::intersection_right});
                    }
                    if (std::abs((*next_segment)->max().x() - intersection.x()) > EPS) {
                        events.insert({**next_segment, intersection.x() + 3 * EPS, event_status::intersection_right});
                    }
                }
            }
        }

        if (event.status == event_status::end) {
            if (prev_segment != active_segments.begin() && next_segment != active_segments.end()) {
                if (intersect(**prev_segment, **next_segment)) {
                    auto intersection = intersectSegments(**prev_segment, **next_segment);
                    result.emplace_back(intersection, (*prev_segment)->id, (*next_segment)->id);
                    if ((intersection.x() >= event.x) && ((intersection.x() - event.x) > 5 * EPS || event.status != event_status::intersection_right)) {
                        if (std::abs((*prev_segment)->max().x() - intersection.x()) > EPS) {
                            events.insert({**prev_segment, intersection.x() + 3 * EPS, event_status::intersection_right});
                        }
                        if (std::abs((*next_segment)->max().x() - intersection.x()) > EPS) {
                            events.insert({**next_segment, intersection.x() + 3 * EPS, event_status::intersection_right});
                        }
                    }
                }
            }
            if (event.status == event_status::end) {
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
        }
        events.erase(event_it);
    }

    return result;
}

} // namespace gkernel
