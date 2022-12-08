#ifndef __GKERNEL_HPP_INTERSECTION
#define __GKERNEL_HPP_INTERSECTION

#include "containers.hpp"

#include <tuple>
#include <functional>

namespace gkernel {
    // Callable returns bool value indicating whether to continue searching for intersection points
    using Callback = std::function<bool(const SegmentsSet&, const Segment&, const Segment&, const Segment&)>;

    enum segments_relation_type {
        not_intersect,
        point,
        segment,
        parallel
    };

    // if segments_relation_type is not_intersect, Segment has ub value
    std::tuple<Segment, segments_relation_type> intersectSegments(const Segment& first, const Segment& second);

    void intersectSetSegments(const SegmentsSet& segments, Callback&& notify);

} // namespace gkernel
#endif // __GKERNEL_HPP_INTERSECTION
