#ifndef __GKERNEL_HPP_AREA_ANALYSIS
#define __GKERNEL_HPP_AREA_ANALYSIS

#include "intersection.hpp"
#include "containers.hpp"

#include <tuple>
#include <functional>

namespace gkernel {

class AreaAnalyzer {
public:
    AreaAnalyzer() = delete;

    static SegmentsLayer findAreas(const SegmentsLayer& layer) {
        auto neighbours = findSegmentsNeighbours(layer);
        SegmentsLayer result = markAreas(neighbours);
        return result;
    }
    enum direction {
        top = 0,
        bottom = 1
    };

private:
    static void internalFindSegmentsNeighbours(const SegmentsLayer& layer, SegmentsSet& result, bool rotated);
    static void bypassNeighbours(std::vector<gkernel::label_data_type>& neighbours, std::vector<std::size_t>& history, std::vector<gkernel::label_data_type>& segment_layer_ids,
        SegmentsSet& result, gkernel::label_data_type start_idx, direction direction);
public:
    static std::pair<SegmentsSet, SegmentsSet> findSegmentsNeighbours(const SegmentsLayer& layer);
    static SegmentsLayer markAreas(const std::pair<SegmentsSet, SegmentsSet>& neighbours);

    template<typename Callable>
    static SegmentsLayer markAreasAndFilter(const SegmentsLayer& layer, Callable callable) {
        auto areas = findAreas(layer);
        std::vector<Segment> result;
        std::vector<std::size_t> ids;
        for (std::size_t idx = 0; idx < areas.size(); ++idx) {
            if (callable(areas, layer[idx])) {
                result.emplace_back(areas[idx]);
                ids.emplace_back(idx);
            }
        }
        if (result.empty()) {
            return SegmentsLayer();
        }
        SegmentsSet layer_result(result);
        layer_result.set_labels_types(layer.get_label_types());
        for (std::size_t idx = 0; idx < ids.size(); ++idx) {
            for (auto label : layer.get_label_types()) {
                layer_result.set_label_value(label, layer_result[idx], layer.get_label_value(label, layer[ids[idx]]));
            }
        }
        return layer_result;
    }
};

} // namespace gkernel
#endif // __GKERNEL_HPP_AREA_ANALYSIS
