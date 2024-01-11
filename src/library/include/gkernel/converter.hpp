#ifndef __GKERNEL_HPP_CONVERTER
#define __GKERNEL_HPP_CONVERTER

#include "containers.hpp"
#include "intersection.hpp"

namespace gkernel {

class Converter {
    Converter() = delete;

    static SegmentsLayer _convertToSegmentsLayer(const SegmentsSet& orig_segments,
                                                 const std::vector<IntersectionSegment>& intersections);

public:
    template<typename Callable>
    static SegmentsSet convertToSegmentsSet(const SegmentsSet& segments, Callable filter) {
        SegmentsLayer converted_layer = convertToSegmentsLayer(segments);
        std::vector<Segment> result_segments;
        result_segments.reserve(converted_layer.size());
        for (std::size_t idx = 0; idx < converted_layer.size(); ++idx) {
            auto& segment = converted_layer[idx];
            if (filter(converted_layer, segment)) {
                result_segments.push_back(segment);
            }
        }

        return result_segments;
    }
    static SegmentsLayer convertToSegmentsLayer(const SegmentsSet& segments);
    static SegmentsLayer convertToSegmentsLayer(const CircuitsSet& circuits);

    static CircuitsLayer convertToCircuitsLayer(const CircuitsSet& circuits);

    static CircuitsSet convertToCircuitsSet(const CircuitsLayer& circuits);
    static SegmentsSet convertToSegmentsSet(const SegmentsLayer& segments);

    static SegmentsSet mergeCircuitsLayers(const CircuitsLayer& first_layer, const CircuitsLayer& second_layer);

private:
    static SegmentsLayer convertToSegmentsLayer(const SegmentsSet& orig_segments,
                                                const std::vector<IntersectionSegment>& intersections) {
        return _convertToSegmentsLayer(orig_segments, intersections);
    }
};

} // namespace gkernel
#endif // __GKERNEL_HPP_CONVERTER
