#ifndef __GKERNEL_HPP_INTERSECTION
#define __GKERNEL_HPP_INTERSECTION

#include "objects.hpp"
#include "containers.hpp"

#include <tuple>
#include <functional>

namespace gkernel {
    using segment_id = uint64_t;
    // using segments_intersection = std::tuple<Segment, segment_id, segment_id>;
    // Callable returns bool value indicating whether to continue searching for intersection points
    using Callback = std::function<bool(const Segment&, const Segment&, const Segment&)>;

    enum intersection_type {
        none,
        point,
        segment,
        parallel
    };

    // if intersection_type is none, Segment has ub value
    // Segment intersectSegments(const Segment& first, const Segment& second);
    Segment intersectSegments(const Segment& first, const Segment& second);

    void intersectSetSegmentsBruteForce(const SegmentsSet& segments, Callback&& notify);
    void intersectSetSegments(const SegmentsSet& segments, Callback&& notify);

} // namespace gkernel
#endif // __GKERNEL_HPP_INTERSECTION
