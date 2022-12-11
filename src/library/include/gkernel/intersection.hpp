#ifndef __GKERNEL_HPP_INTERSECTION
#define __GKERNEL_HPP_INTERSECTION

#include "containers.hpp"

#include <vector>
#include <functional>

namespace gkernel {

struct IntersectionPoint {
    IntersectionPoint(const Point& point, const Segment* first, const Segment* second) : _point(point), _first_segment(first), _second_segment(second) {}

    const Point& point() const {
        return _point;
    }

    const Segment* first() const {
        return _first_segment;
    }

    const Segment* second() const {
        return _second_segment;
    }

private:
    Point _point;
    const Segment* _first_segment;
    const Segment* _second_segment;
};

class Intersection {
public:
    Intersection() = delete;
    static Point intersectSegments(const Segment& first, const Segment& second);
    static std::vector<IntersectionPoint> intersectSetSegments(const SegmentsSet& segments);
private:
    enum event_status {
        start = 2,
        vertical = 1,
        end = 0
    };

    struct Event {
        Event(const gkernel::Segment& segment, gkernel::data_type x, event_status status) : segment(&segment), x(x), status(status) {}

        bool operator<(const Event& other) const {
            if (x != other.x) return x < other.x;
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
