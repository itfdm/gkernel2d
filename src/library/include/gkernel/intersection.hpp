#ifndef __GKERNEL_HPP_INTERSECTION
#define __GKERNEL_HPP_INTERSECTION

#include "containers.hpp"

#include <vector>
#include <functional>

namespace gkernel {

constexpr static double EPS = 1e-9;

static inline double get_sweeping_line_y(const Segment& segment, double x) {
    double k = (segment.end().y() - segment.start().y()) / (segment.end().x() - segment.start().x());
    double m = segment.start().y() - k * segment.start().x();
    return k * (x + EPS) + m;
}

static inline double get_sweeping_line_y2(const Segment& segment, double x) {
    double k = (segment.end().y() - segment.start().y()) / (segment.end().x() - segment.start().x());
    double m = segment.start().y() - k * segment.start().x();
    return k * (x) + m;
}

struct IntersectionPoint {
    IntersectionPoint(const Point& point, segment_id first_segment_id, segment_id second_segment_id) :
        _point(point),
        _first_segment_id(first_segment_id),
        _second_segment_id(second_segment_id) {}

    const Point& point() const {
        return _point;
    }

    segment_id first() const {
        return _first_segment_id;
    }

    segment_id second() const {
        return _second_segment_id;
    }

private:
    Point _point;
    segment_id _first_segment_id;
    segment_id _second_segment_id;
};

class Intersection {
public:
    Intersection() = delete;
    static Point intersectSegments(const Segment& first, const Segment& second);
    static std::vector<IntersectionPoint> intersectSetSegments(const SegmentsSet& segments);
private:
    enum event_status {
        start = 4,
        intersection_right = 3,
        intersection_left = 2,
        vertical = 1,
        end = 0
    };

    struct Event {
        Event(const gkernel::Segment& segment, gkernel::data_type x, event_status status) : segment(&segment), x(x), status(status) {}

        bool operator<(const Event& other) const {
            if (std::abs(x - other.x) > 3 * EPS) return x < other.x;
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
