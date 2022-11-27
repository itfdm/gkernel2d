#include "gkernel/intersection.hpp"
#include "gkernel/rbtree.hpp"

namespace gkernel {

constexpr double EPS = 1E-9;

struct event {
	const Segment* seg;
    gkernel::data_type x;
    bool is_start;

    event(const gkernel::Segment& seg, gkernel::data_type x, bool is_start) : seg(&seg), x(x), is_start(is_start) {}

	bool operator<(const event& other) const {
		if (std::abs(x - other.x) > EPS) return x < other.x;
		if (is_start != other.is_start) return static_cast<int8_t>(is_start) > static_cast<int8_t>(other.is_start);
        return this->seg->id < other.seg->id;
	}
};

inline bool intersect1d(gkernel::data_type l1, gkernel::data_type r1,
                        gkernel::data_type l2, gkernel::data_type r2) {
    if (l1 > r1) std::swap(l1, r1);
    if (l2 > r2) std::swap(l2, r2);
    return std::max(l1, l2) <= std::min(r1, r2) + EPS;
}

inline int vec(const gkernel::Point &a, const gkernel::Point &b,
               const gkernel::Point &c) {
    gkernel::data_type s = (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
    return abs(s) < EPS ? 0 : s > 0 ? 1 : -1;
}

bool intersect(const gkernel::Segment &a, const gkernel::Segment &b) {
    return intersect1d(a.start().x(), a.end().x(), b.start().x(), b.end().x()) &&
           intersect1d(a.start().y(), a.end().y(), b.start().y(), b.end().y()) &&
           vec(a.start(), a.end(), b.start()) * vec(a.start(), a.end(), b.end()) <= 0 &&
           vec(b.start(), b.end(), a.start()) * vec(b.start(), b.end(), a.end()) <= 0;
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
    } else {
        if (a.start().x() <= b.start().x() && b.end().x() <= a.end().x()) {

            return gkernel::Segment({b.start().x(), b.start().y()},
                                    {b.end().x(), b.end().y()});
        } else if (a.start().x() <= b.start().x() && a.end().x() <= b.end().x()) {
            return gkernel::Segment({b.start().x(), b.start().y()},
                                    {a.end().x(), a.end().y()});
        } else if (b.start().x() <= a.start().x() && a.end().x() <= b.end().x()) {
            return gkernel::Segment({a.start().x(), a.start().y()},
                                    {a.end().x(), a.end().y()});
        } else if (b.start().x() <= a.start().x() && b.end().x() <= a.end().x()) {
            return gkernel::Segment({a.start().x(), a.start().y()},
                                    {b.end().x(), b.end().y()});
        }
    }
}

// double get_y(const gkernel::Segment& segment, double x) {
//     double alpha = (segment.end().x() - x - EPS) / (segment.end().x() - segment.start().x());
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

double get_y(const gkernel::Segment& segment, double x) {
    double k = (segment.end().y() - segment.start().y()) / (segment.end().x() - segment.start().x());
    double m = segment.start().y() - k * segment.start().x();
    return k * x + m;
}

void intersectSetSegments(const SegmentsSet& segments, Callback&& notify) {
    if (segments.size() == 0) {
        return;
    }
	std::set<event> events;
    double x = 0;
    auto compare_segments = [&x](const Segment& c1, const Segment& c2) -> bool {
        double Y1 = get_y(c1, x);
        double Y2 = get_y(c2, x);
        if (Y1 - Y2 != 0) return Y1 < Y2;
        return c1.id < c2.id;
    };
    using tree_type = RBTree<Segment, decltype(compare_segments)>;
    tree_type active_segments(compare_segments);

	for (std::size_t idx = 0; idx < segments.size(); ++idx) {
        auto& segment = segments[idx];
		events.insert({segment, std::min(segment.start().x(), segment.end().x()), true});
		events.insert({segment, std::max(segment.start().x(), segment.end().x()), false});
	}

    std::vector<Segment> segments_vector;
    segments_vector.reserve(events.size());

    while (events.size() > 0) {
        auto event = *events.begin();
        if (event.x != x) {
            while (active_segments.size()) {
                auto it = active_segments.begin();
                segments_vector.push_back(*it);
                active_segments.erase(it);
            }
            x = event.x;
            for (auto& elem : segments_vector) {
                active_segments.insert(elem);
            }
            segments_vector.clear();
        }

        if (event.is_start) {
            auto insert_result = active_segments.insert(*event.seg);
            double k = (event.seg->end().y() - event.seg->start().y()) / (event.seg->end().x() - event.seg->start().x());
            auto prev_segment = active_segments.find_prev(*event.seg);
            if (prev_segment.second == tree_type::state::exists) {
                if (intersect(*event.seg, prev_segment.first)) {
                    auto intersection = intersectSegments(*event.seg, prev_segment.first);
                    notify(*event.seg, prev_segment.first, intersection);
                }
                if (k < 0) {
                    auto prev_segment_iter = active_segments.find(prev_segment.first);
                    while (prev_segment_iter != active_segments.begin()) {
                        --prev_segment_iter;
                        if (intersect(*event.seg, *prev_segment_iter)) {
                            auto intersection = intersectSegments(*event.seg, *prev_segment_iter);
                            notify(*event.seg, *prev_segment_iter, intersection);
                        }
                    }
                }
            }
            auto next_segment = active_segments.find_next(*event.seg);
            if (next_segment.second == tree_type::state::exists) {
                if (intersect(*event.seg, next_segment.first)) {
                    auto intersection = intersectSegments(*event.seg, next_segment.first);
                    notify(*event.seg, next_segment.first, intersection);
                }
                if (k >= 0) {
                    auto next_segment_iter = active_segments.find(next_segment.first);
                    while (next_segment_iter != active_segments.end()) {
                        if (intersect(*event.seg, *next_segment_iter)) {
                            auto intersection = intersectSegments(*event.seg, *next_segment_iter);
                            notify(*event.seg, *next_segment_iter, intersection);
                        }
                        ++next_segment_iter;
                    }
                }
            }
        } else {
            auto prev_segment = active_segments.find_prev(*event.seg);
            auto next_segment = active_segments.find_next(*event.seg);
            if (prev_segment.second == tree_type::state::exists && next_segment.second == tree_type::state::exists) {
                if (intersect(prev_segment.first, next_segment.first)) {
                    auto intersection = intersectSegments(prev_segment.first, next_segment.first);
                    notify(prev_segment.first, next_segment.first, intersection);
                }
            }
            active_segments.erase(*event.seg);
        }
        events.erase(events.begin());
    }
}

} // namespace gkernel
