#include "gkernel/intersection.hpp"
#include "gkernel/rbtree.hpp"

namespace gkernel {

inline int get_area(const Point& a, const Point& b, const Point& c) {
    data_type area = (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
    return area == 0 ? 0 : area > 0 ? 1 : -1;
}

inline bool intersect(const Segment& first, const Segment& second) {
    return get_area(first.start(), first.end(), second.start()) * get_area(first.start(), first.end(), second.end()) <= 0 &&
           get_area(second.start(), second.end(), first.start()) * get_area(second.start(), second.end(), first.end()) <= 0;
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
}

std::vector<IntersectionPoint> Intersection::intersectSetSegments(const SegmentsSet& segments) {
    std::vector<IntersectionPoint> result;

    if (segments.size() == 0) {
        return result;
    }

	std::set<Event> events;
    double x_sweeping_line = 0;

    auto compare_segments = [&x_sweeping_line](const Segment* first, const Segment* second) -> bool {
        double y1 = get_sweeping_line_y(*first, x_sweeping_line);
        double y2 = get_sweeping_line_y(*second, x_sweeping_line);
        if (y1 - y2 != 0) return y1 < y2;
        return first->id < second->id;
    };

    auto compare_segments_ids_pairs = [](const std::pair<segment_id, segment_id>& first, const std::pair<segment_id, segment_id>& second) -> bool {
        if ((first.first == second.first || first.first == second.second) && (first.second == second.first || first.second == second.second)) {
            return false;
        }
        if (first.first != second.first) {
            return first.first < second.first;
        }
        return first.second < second.second;
    };

    std::set<std::pair<segment_id, segment_id>, decltype(compare_segments_ids_pairs)> checked_pairs(compare_segments_ids_pairs);

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

    while (events.size() > 0) {
        auto event = *events.begin();
        temp_segments.clear();
        if (event.status == event_status::intersection_right) {
            auto event_it = events.begin();
            while (event_it != events.end() && event_it->x == event.x && event_it->status == event_status::intersection_right) {
                temp_segments.push_back(event_it->segment);
                active_segments.erase(event_it->segment);
                ++event_it;
            }
            x_sweeping_line = event.x;
            for (auto& segment : temp_segments) {
                active_segments.insert(segment);
            }
        } else {
            x_sweeping_line = event.x;
        }

        if (event.status == event_status::vertical) {
            for (auto item : active_segments) {
                if (intersect(*event.segment, *item)) {
                    auto intersection = intersectSegments(*event.segment, *item);
                    result.emplace_back(intersection, event.segment->id, item->id);
                }
            }
            events.erase(events.begin());
            continue;
        }

        auto insert_result = active_segments.insert(event.segment);

        auto prev_segment = insert_result.first;
        if (prev_segment != active_segments.begin()) {
            --prev_segment;
            if (intersect(*event.segment, **prev_segment)) {
                auto intersection = intersectSegments(*event.segment, **prev_segment);
                result.emplace_back(intersection, event.segment->id, (*prev_segment)->id);
                if (checked_pairs.insert(std::make_pair(event.segment->id, (*prev_segment)->id)).second) {
                    events.insert({*event.segment, intersection.x() - 2 * EPS, event_status::intersection_left});
                    events.insert({**prev_segment, intersection.x() - 2 * EPS, event_status::intersection_left});
                    events.insert({*event.segment, intersection.x() + 2 * EPS, event_status::intersection_right});
                    events.insert({**prev_segment, intersection.x() + 2 * EPS, event_status::intersection_right});
                }
            }
        }

        auto next_segment = insert_result.first;
        ++next_segment;
        if (next_segment != active_segments.end()) {
            if (intersect(*event.segment, **next_segment)) {
                auto intersection = intersectSegments(*event.segment, **next_segment);
                result.emplace_back(intersection, event.segment->id, (*next_segment)->id);
                if (checked_pairs.insert(std::make_pair(event.segment->id, (*next_segment)->id)).second) {
                    events.insert({*event.segment, intersection.x() - 2 * EPS, event_status::intersection_left});
                    events.insert({**next_segment, intersection.x() - 2 * EPS, event_status::intersection_left});
                    events.insert({*event.segment, intersection.x() + 2 * EPS, event_status::intersection_right});
                    events.insert({**next_segment, intersection.x() + 2 * EPS, event_status::intersection_right});
                }
            }
        }

        if (event.status == event_status::end) {
            if (prev_segment != active_segments.begin() && next_segment != active_segments.end()) {
                if (intersect(**prev_segment, **next_segment)) {
                    auto intersection = intersectSegments(**prev_segment, **next_segment);
                    result.emplace_back(intersection, (*prev_segment)->id, (*next_segment)->id);
                    if (checked_pairs.insert(std::make_pair((*prev_segment)->id, (*next_segment)->id)).second) {
                        events.insert({**next_segment, intersection.x() - 2 * EPS, event_status::intersection_left});
                        events.insert({**prev_segment, intersection.x() - 2 * EPS, event_status::intersection_left});
                        events.insert({**next_segment, intersection.x() + 2 * EPS, event_status::intersection_right});
                        events.insert({**prev_segment, intersection.x() + 2 * EPS, event_status::intersection_right});
                    }
                }
            }
            if (event.status == event_status::end) {
                active_segments.erase(event.segment);
            }
        }
        events.erase(events.begin());
    }

    return result;
}

} // namespace gkernel
