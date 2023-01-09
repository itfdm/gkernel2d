#ifndef __GKERNEL_HPP_CONVERTER
#define __GKERNEL_HPP_CONVERTER

#include "containers.hpp"
#include "intersection.hpp"

namespace gkernel {
    /**
     * @brief Класс для преобразования контейнеров одного класса в контейнеры другого класса.
     * Методы класса создают новые контейнеры на основе содержимого входных контейнеров, состояние входных данных при этом не меняется.
     */
    class Converter {
        Converter() = delete;
        static SegmentsLayer _convertToSegmentsLayer(const SegmentsSet& orig_segments,
            const std::vector<IntersectionPoint>& intersections);

    public:
        /**
         * @brief Преобразует набор отрезков в слой отрезков.
         *
         * @param segments Набор отрезков.
         * @return SegmentsLayer слой отрезков, построенный из отрезков из segments.
         */
        static SegmentsLayer convertToSegmentsLayer(const SegmentsSet& segments);
        /**
         * @brief Преобразует набор контуров в слой отрезков.
         *
         * @param circuits Набор контуров.
         * @return SegmentsLayer слой отрезков, построенный из отрезков circuits.
         */
        static SegmentsLayer convertToSegmentsLayer(const CircuitsSet& circuits);
        /**
         * @brief Преобразует набор контуров в слой контуров.
         *
         * @param circuits Набор контуров.
         * @return CircuitsLayer слой контуров, построенный из набора контуров.
         */
        static CircuitsLayer convertToCircuitsLayer(const CircuitsSet& circuits);
        /**
         * @brief Преобразует слой контуров в набор контуров.
         *
         * @param circuits Слой контуров.
         * @return CircuitsSet набор контуров, построенный из слоя контуров circuits.
         */
        static CircuitsSet convertToCircuitsSet(const CircuitsLayer& circuits);
        /**
         * @brief Преобразует слой отрезков в набор отрезков.
         *
         * @param segments Слой отрезков.
         * @return SegmentsSet набор отрезков, построенный из слоя отрезков segments.
         */
        static SegmentsSet convertToSegmentsSet(const SegmentsLayer& segments);
        /**
         * @brief Объединяет отрезки из двух слоёв контуров в один набор отрезков и ставит метку каждому отрезку, указывающую какому контуру принадлежит отрезок.
         * Если отрезок принадлежит first_layer, то метка 0, иначе - 1
         *
         * @param first_layer Первый слой контуров.
         * @param second_layer Второй слой контуров.
         * @return SegmentsSet Набор контуров, построенный из отрезков обоих слоёв контуров.
         */
        static SegmentsSet mergeCircuitsLayers(const CircuitsLayer& first_layer, const CircuitsLayer& second_layer);

    private:
        static SegmentsLayer convertToSegmentsLayer(const SegmentsSet& orig_segments,
            const std::vector<IntersectionPoint>& intersections) {
            return _convertToSegmentsLayer(orig_segments, intersections);
        }
    };

} // namespace gkernel
#endif // __GKERNEL_HPP_CONVERTER
