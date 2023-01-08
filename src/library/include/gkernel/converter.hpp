#ifndef __GKERNEL_HPP_CONVERTER
#define __GKERNEL_HPP_CONVERTER

#include "containers.hpp"
#include "intersection.hpp"

namespace gkernel {

class Converter {
    Converter() = delete;

    static SegmentsLayer _convertToSegmentsLayer(const SegmentsSet& orig_segments,
                                                 const std::vector<IntersectionPoint>& intersections);

public:
    static SegmentsLayer convertToSegmentsLayer(const SegmentsSet& segments);
    static SegmentsLayer convertToSegmentsLayer(const CircuitsSet& circuits);

    static CircuitsLayer convertToCircuitsLayer(const CircuitsSet& circuits);

    static CircuitsSet convertToCircuitsSet(const CircuitsLayer& circuits);
    static SegmentsSet convertToSegmentsSet(const SegmentsLayer& segments);

    static SegmentsSet mergeCircuitsLayers(const CircuitsLayer& first_layer, const CircuitsLayer& second_layer);

private:
    static SegmentsLayer convertToSegmentsLayer(const SegmentsSet& orig_segments,
                                                const std::vector<IntersectionPoint>& intersections) {
        return _convertToSegmentsLayer(orig_segments, intersections);
    }
};

} // namespace gkernel
#endif // __GKERNEL_HPP_CONVERTER
