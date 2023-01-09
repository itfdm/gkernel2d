#ifndef __GKERNEL_HPP_AREA_ANALYSIS
#define __GKERNEL_HPP_AREA_ANALYSIS

#include "intersection.hpp"
#include "containers.hpp"

#include <tuple>
#include <functional>

namespace gkernel {

    /**
     * @brief Класс, решающий задачу анализа областей.
     *
     */
    class AreaAnalyzer
    {
    public:
        AreaAnalyzer() = delete;

        static SegmentsLayer findAreas(const SegmentsLayer& layer) {
            SegmentsLayer neighbours = findSegmentsNeighbours(layer);
            SegmentsLayer result = markAreas(neighbours);
            return result;
        }

        static SegmentsLayer findSegmentsNeighbours(const SegmentsLayer& layer);
        static SegmentsLayer markAreas(const SegmentsLayer& layer);

        /**
         * @brief Функция отбора отрезков по меткам.
         *
         * @tparam Callable Функция проверки соответствия значений меток.
         * @param layer Слой отрезков, в котором происходит отбор.
         * @param callable
         * @return SegmentsLayer Слой отрезков, в котором отрезки соответствуют заданным значениям меток.
         */
        template <typename Callable>
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
