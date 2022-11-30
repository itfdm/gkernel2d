#ifndef __GKERNEL_HPP_INTERSECTION
#define __GKERNEL_HPP_INTERSECTION

#include "objects.hpp"
#include "containers.hpp"

#include <tuple>
#include <functional>

namespace gkernel
{
    using segment_id = uint64_t;
    // using segments_intersection = std::tuple<Segment, segment_id, segment_id>;
    // Callable returns bool value indicating whether to continue searching for intersection points
    using Callback = std::function<bool(const Segment &, const Segment &, const Segment &)>;
    using Labeler = std::function<void(SegmentsSet &)>;

    enum intersection_type
    {
        none,
        point,
        segment,
        parallel
    };

    // if intersection_type is none, Segment has ub value
    /**
     * @brief Ищет пересечения двух отрезков.
     *
     * @param first первый отрезок
     * @param second второй отрезок
     * @return std::tuple<Segment, intersection_type> возвращает пару: пересечение-тип пересечения.
     *         @n Возможные результаты:
     *         @n 1. Пересечений нет, отрезки не параллельны. Вернётся отрезок, созданный конструктором по умолчанию, значение intersection_type=none
     *         @n 2. Пресечение в одной точке. Вернётся отрезок с началом и концом в точке пересечения, значение intersection_type=point
     *         @n 3. Наложение отрезков. Вернётся отрезок, в котором происходит наложение, значение intersection_type=segment
     *         @n 4. Отрезки параллельны. Вернётся отрезок, созданный конструктором по умолчанию, значение intersection_type=parallel
     */
    std::tuple<Segment, intersection_type> intersectSegments(const Segment &first, const Segment &second);

    /**
     * @brief Для работы алгоритма поиска пересечений набора отрезков необходимо использовать функцию обратного вызова notify и функцию разметки рёбер labeler.
     * @n @b Функция @b обратного @b вызова notify
     * @n @b Вход: Segment, Segment, Segment
     * @n @b Выход: @b bool result
     * @n Функция корректирует процесс поиска точек пересечений. Получив два пересекающихся отрезка и их область пересечения, возвращает булево значение, указывающее нужно ли продолжать поиск: true — продолжить, false —завершить.
     * @n
     * @n @b Функция @b разметки @b рёбер
     * @b Вход: порядковый номер отрезка, ссылка на набор отрезков
     * @n Расставляет значения меток для отрезков.
     *
     * @n
     * @n @b Функция @b поиска @b пересечений @b набора @b отрезков
     * @b Вход: набор отрезков, функция разметки рёбер, функция обратного вызова
     * @n Осуществляет поиск пересечений набора отрезков.
     *
     * @param segments Набор отрезков, на котором осуществляется разметка рёбер.
     * @param notify Функция, управляющая работой алгоритма.
     * @param labeler Функция разметки рёбер.
     */

    void intersectSetSegments(const SegmentsSet &segments, Callback &&notify, Labeler &&labeler);

} // namespace gkernel
#endif // __GKERNEL_HPP_INTERSECTION
