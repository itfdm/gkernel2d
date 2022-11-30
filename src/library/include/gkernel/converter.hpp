#ifndef __GKERNEL_HPP_CONVERTER
#define __GKERNEL_HPP_CONVERTER

#include "containers.hpp"

namespace gkernel
{
    /**
     * @brief Класс, предназначенный для генерации новых контейнеров отрезков на основе уже сформированных контейнеров отрезков другого класса.
     * Методы класса не меняют состояние входных контейнеров, а используют их для конфигурации новых инстансов.
     *
     */

    class Converter
    {
        Converter() = delete;

    public:
        /**
         * @brief Создаёт новый слой отрезков на основе набора отрезков.
         *
         * @param segments Набор отрезков.
         * @return SegmentsLayer слой отрезков, построенный из отрезков набора.
         */
        static SegmentsLayer convertToSegmentsLayer(const SegmentsSet &segments);

        /**
         * @brief Создаёт новый слой отрезков на основе набора контуров.
         *
         * @param circuits Набор контуров.
         * @return SegmentsLayer слой отрезков, построенный из отрезков набора.
         */
        static SegmentsLayer convertToSegmentsLayer(const CircuitsSet &circuits);

        /**
         * @brief Создаёт новый слой контуров на основе набора контуров.
         *
         * @param circuits Набор контуров.
         * @return CircuitsLayer слой контуров, построенный из набора контуров.
         */

        static CircuitsLayer convertToCircuitsLayer(const CircuitsSet &circuits);

        /**
         * @brief Создаёт новый набор контуров на основе слоя контуров.
         *
         * @param circuits Слой контуров.
         * @return CircuitsSet набор контуров, построенный из слоя контуров circuits.
         */
        static CircuitsSet convertToCircuitsSet(const CircuitsLayer &circuits);

        /**
         * @brief Создаёт новый набор отрезков на основе слоя отрезков.
         *
         * @param segments Слой отрезков.
         * @return SegmentsSet набор отрезков, построенный из слоя отрезков.
         */
        static SegmentsSet convertToSegmentsSet(const SegmentsLayer &segments);
    };

} // namespace gkernel
#endif // __GKERNEL_HPP_CONVERTER
