#ifndef __GKERNEL_HPP_AREA_ANALYSIS
#define __GKERNEL_HPP_AREA_ANALYSIS

#include "containers.hpp"

#include <tuple>
#include <functional>

namespace gkernel
{

    class AreaAnalysis
    {
    public:
        AreaAnalysis() = delete;
        static SegmentsLayer findAreas(const SegmentsLayer &layer)
        {
            SegmentsLayer neighbours = findSegmentsNeighbours(layer);
            SegmentsLayer result = markAreas(neighbours);
            return result;
        }
        static SegmentsLayer findSegmentsNeighbours(const SegmentsLayer &layer);
        static SegmentsLayer markAreas(const SegmentsLayer &layer);
        static SegmentsLayer parallelMarkAreas(const SegmentsLayer &layer);
    };

} // namespace gkernel
#endif // __GKERNEL_HPP_AREA_ANALYSIS
