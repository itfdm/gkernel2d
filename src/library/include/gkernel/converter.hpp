#ifndef __GKERNEL_HPP_CONVERTER
#define __GKERNEL_HPP_CONVERTER

#include "containers.hpp"

namespace gkernel {

class Converter {
    Converter() = delete;

public:
    static SegmentsLayer convertToSegmentsLayer(const SegmentsSet& segments);
    static SegmentsLayer convertToSegmentsLayer(const CircuitsSet& circuits);

    static CircuitsLayer convertToCircuitsLayer(const CircuitsSet& circuits);

    static CircuitsSet convertToCircuitsSet(const CircuitsLayer& circuits);
    static SegmentsSet convertToSegmentsSet(const SegmentsLayer& segments);

// private: temp solution for testing
    static SegmentsLayer convertToSegmentsLayer(const SegmentsSet& orig_segments,
                                                const std::vector<std::tuple<Segment, segment_id, segment_id>>& intersections);
};

} // namespace gkernel
#endif // __GKERNEL_HPP_CONVERTER
