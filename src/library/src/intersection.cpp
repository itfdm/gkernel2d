#include "gkernel/intersection.hpp"
#include "gkernel/rbtree.hpp"

namespace gkernel {

constexpr double EPS = 1E-9;

enum event_status {
    start = 4,
    intersection_right = 3,
    intersection_left = 2,
    vertical = 1,
    end = 0
};

struct event {
	Segment* seg;
    gkernel::data_type x;
    event_status status;

    event(gkernel::Segment& seg, gkernel::data_type x, event_status status) : seg(&seg), x(x), status(status) {}

	bool operator<(const event& other) const {
		if (std::abs(x - other.x) > 3 * EPS) return x < other.x;
		if (status != other.status) return static_cast<int8_t>(status) > static_cast<int8_t>(other.status);
        return this->seg->id < other.seg->id;
	}
};

inline int area(const gkernel::Point &a, const gkernel::Point &b,
               const gkernel::Point &c) {
    gkernel::data_type s = (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
    return abs(s) < EPS ? 0 : s > 0 ? 1 : -1;
}

bool intersect(const gkernel::Segment &a, const gkernel::Segment &b) {
    return area(a.start(), a.end(), b.start()) * area(a.start(), a.end(), b.end()) <= 0 &&
           area(b.start(), b.end(), a.start()) * area(b.start(), b.end(), a.end()) <= 0;
}

// intersect two segments
Segment intersectSegments(const Segment& a, const Segment& b) {
    gkernel::data_type a1 = a.end().y() - a.start().y();
    gkernel::data_type b1 = a.start().x() - a.end().x();
    gkernel::data_type c1 = a.start().y() * a.end().x() -
                            a.start().x() * a.end().y();

    gkernel::data_type a2 = b.end().y() - b.start().y();
    gkernel::data_type b2 = b.start().x() - b.end().x();
    gkernel::data_type c2 = b.start().y() * b.end().x() -
                            b.start().x() * b.end().y();
    if ((a1 * b2 - a2 * b1) != 0) {
        data_type x = (data_type)(b1 * c2 - b2 * c1) / (data_type)(a1 * b2 - a2 * b1);
        data_type y = (data_type)(a2 * c1 - a1 * c2) / (data_type)(a1 * b2 - a2 * b1);
        return gkernel::Segment({x, y}, {x, y});
    }
    else {
        if (a.start() == b.start())
            return gkernel::Segment({ a.start().x(), a.start().y() }, { a.start().x(), a.start().y() });
        else if (a.end() == b.end())
            return gkernel::Segment({ a.end().x(), a.end().y() }, { a.end().x(), a.end().y() });
        else if (a.start() == b.end())
            return gkernel::Segment({ a.start().x(), a.start().y() }, { a.start().x(), a.start().y() });
        else if (a.end() == b.start())
            return gkernel::Segment({ a.end().x(), a.end().y() }, { a.end().x(), a.end().y() });
    }
}

// inline double get_y(const gkernel::Segment& segment, double x) {
//     double alpha = (segment.end().x() - x - 2 * EPS) / (segment.end().x() - segment.start().x());
//     return alpha * segment.start().y() + (1 - alpha) * segment.end().y();
// }

void intersectSetSegmentsBruteForce(const SegmentsSet& segments, Callback&& notify) {
    for (size_t i = 0; i < segments.size(); ++i) {
        for (size_t j = i + 1; j < segments.size(); ++j) {
            if (intersect(segments[i], segments[j])) {
                notify(segments[i], segments[j], intersectSegments(segments[i], segments[j]));
            }
        }
    }
}

inline double get_y(const gkernel::Segment& segment, double x) {
    double k = (segment.end().y() - segment.start().y()) / (segment.end().x() - segment.start().x());
    double m = segment.start().y() - k * segment.start().x();
    return k * (x + 2 * EPS) + m;
}

void intersectSetSegments(SegmentsSet& segments, Callback&& notify) {
    if (segments.size() == 0) {
        return;
    }
	std::set<event> events;
    double x = 0;
    auto compare_segments = [&x](const Segment* c1, const Segment* c2) -> bool {
        double Y1 = get_y(*c1, x);
        double Y2 = get_y(*c2, x);
        if (Y1 - Y2 != 0) return Y1 < Y2;
        return c1->id < c2->id;
    };

    auto compare_segments_pairs = [](const std::pair<Segment*, Segment*>& first, const std::pair<Segment*, Segment*>& second) -> bool {
        if ((first.first == second.first || first.first == second.second) && (first.second == second.first || first.second == second.second)) {
            return false;
        }
        if (first.first != second.first) {
            return reinterpret_cast<uintptr_t>(first.first) < reinterpret_cast<uintptr_t>(second.first);
        }
        return reinterpret_cast<uintptr_t>(first.second) < reinterpret_cast<uintptr_t>(second.second);
    };
    std::set<std::pair<Segment*, Segment*>, decltype(compare_segments_pairs)> checked_pairs(compare_segments_pairs);

    using tree_type = RBTree<Segment*, decltype(compare_segments)>;
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

    std::vector<Segment*> temp_segments;
    temp_segments.reserve(segments.size());

    while (events.size() > 0) {
        auto event = *events.begin();
        // TODO: check neighbors only for start and intersection events?
        temp_segments.clear();
        if (event.status == event_status::intersection_right) {
            auto event_it = events.begin();
            while (event_it != events.end() && event_it->x == event.x && event_it->status == event_status::intersection_right) {
                temp_segments.push_back(event_it->seg);
                active_segments.erase(event_it->seg);
                ++event_it;
            }
            x = event.x;
            for (auto& segment : temp_segments) {
                active_segments.insert(segment);
            }
        } else {
            x = event.x;
        }

        if (event.status == event_status::vertical) {
            for (auto item : active_segments) {
                if (intersect(*event.seg, *item)) {
                    auto intersection = intersectSegments(*event.seg, *item);
                    bool result = notify(*event.seg, *item, intersection);
                    checked_pairs.insert(std::make_pair(event.seg, item));
                }
            }
            events.erase(events.begin());
            continue;
        }

        auto insert_result = active_segments.insert(event.seg);

        auto prev_segment = insert_result.first;
        if (prev_segment != active_segments.begin()) {
            --prev_segment;
            if (intersect(*event.seg, **prev_segment)) {
                auto intersection = intersectSegments(*event.seg, **prev_segment);
                bool result = notify(*event.seg, **prev_segment, intersection);
                if (checked_pairs.insert(std::make_pair(event.seg, *prev_segment)).second) {
                    events.insert({*event.seg, intersection.start().x() - 2 * EPS, event_status::intersection_left});
                    events.insert({**prev_segment, intersection.start().x() - 2 * EPS, event_status::intersection_left});
                    events.insert({*event.seg, intersection.start().x() + 2 * EPS, event_status::intersection_right});
                    events.insert({**prev_segment, intersection.start().x() + 2 * EPS, event_status::intersection_right});
                }
            }
        }
        auto next_segment = insert_result.first;
        ++next_segment;
        if (next_segment != active_segments.end()) {
            if (intersect(*event.seg, **next_segment)) {
                auto intersection = intersectSegments(*event.seg, **next_segment);
                bool result = notify(*event.seg, **next_segment, intersection);
                if (checked_pairs.insert(std::make_pair(event.seg, *next_segment)).second) {
                    events.insert({*event.seg, intersection.start().x() - 2 * EPS, event_status::intersection_left});
                    events.insert({**next_segment, intersection.start().x() - 2 * EPS, event_status::intersection_left});
                    events.insert({*event.seg, intersection.start().x() + 2 * EPS, event_status::intersection_right});
                    events.insert({**next_segment, intersection.start().x() + 2 * EPS, event_status::intersection_right});
                }
            }
        }
        if (event.status == event_status::end) {
            if (prev_segment != active_segments.begin() && next_segment != active_segments.end()) {
                if (intersect(**prev_segment, **next_segment)) {
                    auto intersection = intersectSegments(**prev_segment, **next_segment);
                    bool result = notify(**prev_segment, **next_segment, intersection);
                    if (checked_pairs.insert(std::make_pair(*prev_segment, *next_segment)).second) {
                        events.insert({**next_segment, intersection.start().x() - 2 * EPS, event_status::intersection_left});
                        events.insert({**prev_segment, intersection.start().x() - 2 * EPS, event_status::intersection_left});
                        events.insert({**next_segment, intersection.start().x() + 2 * EPS, event_status::intersection_right});
                        events.insert({**prev_segment, intersection.start().x() + 2 * EPS, event_status::intersection_right});
                    }
                }
            }
            if (event.status == event_status::end) {
                active_segments.erase(event.seg);
            }
        }
        events.erase(events.begin());
    }
}

} // namespace gkernel
