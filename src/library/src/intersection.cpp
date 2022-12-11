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

inline double get_sweeping_line_y(const Segment& segment, double x) {
    double k = (segment.end().y() - segment.start().y()) / (segment.end().x() - segment.start().x());
    double m = segment.start().y() - k * segment.start().x();
    return k * x + m;
}

std::vector<IntersectionPoint> Intersection::intersectSetSegments(const SegmentsSet& segments) {
    std::vector<IntersectionPoint> result;

    if (segments.size() == 0) {
        return result;
    }

    result.reserve(segments.size() * 2);

    std::set<Event> events;
    double x_sweeping_line = 0;

    auto compare_segments = [&x_sweeping_line](const Segment* first, const Segment* second) -> bool {
        double y1 = get_sweeping_line_y(*first, x_sweeping_line);
        double y2 = get_sweeping_line_y(*second, x_sweeping_line);
        if (y1 - y2 != 0) return y1 < y2;
        return first->id < second->id;
    };

    using tree_type = RBTree<const Segment*, decltype(compare_segments)>;
    tree_type active_segments(compare_segments);

    for (std::size_t idx = 0; idx < segments.size(); ++idx) {
        const Segment& segment = segments[idx];
        if (segment.min().x() != segment.max().x()) {
            events.insert({segment, segment.min().x(), event_status::start});
            events.insert({segment, segment.max().x(), event_status::end});
        }
        else {
            events.insert({segment, segment.min().x(), event_status::vertical});
        }
    }

    std::vector<const Segment*> temp_segments;
    temp_segments.reserve(segments.size());

    while (events.size() > 0) {
        auto event = *events.begin();
        if (event.x != x_sweeping_line) {
            while (active_segments.size()) {
                auto it = active_segments.begin();
                temp_segments.push_back(*it);
                active_segments.erase(it);
            }
            x_sweeping_line = event.x;
            for (auto& elem : temp_segments) {
                active_segments.insert(elem);
            }
            temp_segments.clear();
        }

        if (event.status == event_status::vertical) {
            for (auto item : active_segments) {
                if (intersect(*event.segment, *item)) {
                    auto intersection = intersectSegments(*event.segment, *item);
                    result.emplace_back(intersection, event.segment, item);
                }
            }
            events.erase(events.begin());
            continue;
        }

        tree_type::iterator insert_result;

        if (event.status == event_status::start) {
            insert_result = active_segments.insert(event.segment).first;
        } else {
            insert_result = active_segments.find(event.segment);
        }

        auto prev_segment = insert_result;
        while (prev_segment != active_segments.begin()) {
            --prev_segment;
            if (intersect(*event.segment, **prev_segment)) {
                auto intersection = intersectSegments(*event.segment, **prev_segment);
                result.emplace_back(intersection, event.segment, *prev_segment);
            }
        }

        auto next_segment = insert_result;
        ++next_segment;
        while (next_segment != active_segments.end()) {
            if (intersect(*event.segment, **next_segment)) {
                auto intersection = intersectSegments(*event.segment, **next_segment);
                result.emplace_back(intersection, event.segment, *next_segment);
            }
            ++next_segment;
        }

        if (event.status == event_status::end) {
            active_segments.erase(event.segment);
        }
        events.erase(events.begin());
    }

    std::sort(result.begin(), result.end(), [](const IntersectionPoint& first, const gkernel::IntersectionPoint& second) {
        if (first.point().x() != second.point().x()) return first.point().x() < second.point().x();
        return first.point().y() < second.point().y();
    });

    auto to_erase = std::unique(result.begin(), result.end(), [](const IntersectionPoint& first, const gkernel::IntersectionPoint& second) {
        return first.point() == second.point();
    });

    result.erase(to_erase, result.end());

    return result;
}

} // namespace gkernel
