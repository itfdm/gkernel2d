#include "geom_structures_base.hpp"
#include <tuple>

namespace gkernel {
    using segment_id = uint64_t;
    using segments_intersection = std::tuple<Segment, segment_id, segment_id>;

    //true bool value means that Segment is valid, otherwise intersection not found and Segment has ub value
    std::tuple<Segment, bool> intersectSegments(const Segment& first, const Segment& second);

    // Callable returns bool value indicating whether to continue searching for intersection points
    // std::vector<segments_intersection> intersectSetSegments(const SegmentsSet& segments, Callable&& notify);

} // namespace gkernel