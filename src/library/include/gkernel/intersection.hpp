#ifndef __GKERNEL_HPP_INTERSECTION
#define __GKERNEL_HPP_INTERSECTION

#include "containers.hpp"

#include <vector>
#include <functional>

namespace gkernel {

constexpr static double EPS = 1e-9;

static inline double get_sweeping_line_y(const Segment& segment, double x, double eps = 0) {
    double k = (segment.end().y() - segment.start().y()) / (segment.end().x() - segment.start().x());
    double m = segment.start().y() - k * segment.start().x();
    return k * (x + eps) + m;
}

struct IntersectionSegment {
    IntersectionSegment(const Point& first_point, segment_id first_segment_id, segment_id second_segment_id) :
        _is_point(true),
        _first_point(first_point),
        _second_point(first_point),
        _first_segment_id(first_segment_id),
        _second_segment_id(second_segment_id) {}

    IntersectionSegment(const Point& first_point, const Point& second_point, segment_id first_segment_id, segment_id second_segment_id) :
        _is_point(false),
        _first_point(first_point),
        _second_point(second_point),
        _first_segment_id(first_segment_id),
        _second_segment_id(second_segment_id) {}

    const Point& first_point() const {
        return _first_point;
    }

    const Point& second_point() const {
        return _second_point;
    }

    segment_id first_id() const {
        return _first_segment_id;
    }

    segment_id second_id() const {
        return _second_segment_id;
    }

    bool is_point() const {
        return _is_point;
    }

private:
    bool _is_point = false;
    Point _first_point;
    Point _second_point;
    segment_id _first_segment_id;
    segment_id _second_segment_id;
};

class Intersection {
public:
    enum segments_relation {
        none = 0,
        intersect = 1,
        overlap = 2
    };

    Intersection() = delete;
    static Point intersectSegments(const Segment& first, const Segment& second);
    static std::pair<Point, Point> overlapSegments(const Segment& first, const Segment& second);
    static std::vector<IntersectionSegment> intersectSetSegments(const SegmentsSet& segments);
private:
    enum event_status {
        intersection_right = 4,
        end = 3,
        start = 2,
        vertical = 1
    };

    struct Event {
        Event(const gkernel::Segment& segment, gkernel::data_type x, event_status status) : segment(&segment), x(x), status(status) {}

        bool operator<(const Event& other) const {
            if (std::abs(x - other.x) > EPS) return x < other.x;
            if (status == event_status::vertical && other.status == event_status::end) return true;
            if (status == event_status::end && other.status == event_status::vertical) return false;
            if (status != other.status) return static_cast<int8_t>(status) > static_cast<int8_t>(other.status);
            return this->segment->id < other.segment->id;
        }
        const Segment* segment;
        gkernel::data_type x;
        event_status status;
    };
};

} // namespace gkernel
#endif // __GKERNEL_HPP_INTERSECTION
