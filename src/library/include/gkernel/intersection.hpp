#ifndef __GKERNEL_HPP_INTERSECTION
#define __GKERNEL_HPP_INTERSECTION

#include "objects.hpp"
#include "containers.hpp"

#include <tuple>
#include <functional>
#include <set>

namespace gkernel {
    using segment_id = uint64_t;
    // using segments_intersection = std::tuple<Segment, segment_id, segment_id>;
    // Callable returns bool value indicating whether to continue searching for intersection points
    using Callback = std::function<bool(const Segment&, const Segment&, const Segment&)>;
    using Labeler = std::function<void(SegmentsSet&)>;

    enum intersection_type {
        none,
        point,
        segment,
        parallel
    };

    // if intersection_type is none, Segment has ub value
    std::tuple<Segment, intersection_type> intersectSegments(const Segment& first, const Segment& second);

    void intersectSetSegments(const std::vector<SegmentsSet>& segments, Callback&& notify, Labeler&& labeler);

    // TODO: temp function
    std::set<gkernel::Segment> solve(std::vector<gkernel::Segment>& a);

} // namespace gkernel
#endif // __GKERNEL_HPP_INTERSECTION
