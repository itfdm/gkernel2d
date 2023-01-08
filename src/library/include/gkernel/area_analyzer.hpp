#ifndef __GKERNEL_HPP_AREA_ANALYSIS
#define __GKERNEL_HPP_AREA_ANALYSIS

#include "intersection.hpp"
#include "containers.hpp"

#include <tuple>
#include <functional>

namespace gkernel {

class AreaAnalysis {
public:
    AreaAnalysis() = delete;

    static SegmentsLayer findAreas(const SegmentsLayer& layer) {
        SegmentsLayer neighbours = findSegmentsNeighbours(layer);
        SegmentsLayer result = markAreas(neighbours);
        return result;
    }

    static SegmentsLayer findSegmentsNeighbours(const SegmentsLayer& layer);
    static SegmentsLayer markAreas(const SegmentsLayer& layer);

    template<typename Callable>
    static SegmentsLayer filterSegmentsByLabels(const SegmentsLayer& layer, Callable callable) {
        auto areas = findAreas(layer);
        std::vector<Segment> result;
        for (std::size_t idx = 0; idx < areas.size(); ++idx) {
            if (callable(areas, layer[idx])) {
                result.emplace_back(areas[idx]);
            }
        }
        return result;
    }
};

} // namespace gkernel
#endif // __GKERNEL_HPP_AREA_ANALYSIS
