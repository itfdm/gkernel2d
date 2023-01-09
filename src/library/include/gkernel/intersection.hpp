#ifndef __GKERNEL_HPP_INTERSECTION
#define __GKERNEL_HPP_INTERSECTION

#include "containers.hpp"

#include <vector>
#include <functional>

namespace gkernel {

constexpr static double EPS = 1e-9;

static inline double get_sweeping_line_y(const Segment& segment, double x) {
    double k = (segment.end().y() - segment.start().y()) / (segment.end().x() - segment.start().x());
    double m = segment.start().y() - k * segment.start().x();
    return k * (x + EPS) + m;
}

/**
 * @brief Структура, хранящая точку пересечения двух отрезков и индексы этих отрезков.
 *
 */
struct IntersectionPoint
{
    /**
     * @brief Конструктор структуры.
     *
     * @param point Точка пересечения.
     * @param first_segment_id id первого отрезка.
     * @param second_segment_id id второго отрезка.
     */
    IntersectionPoint(const Point& point, segment_id first_segment_id, segment_id second_segment_id): _point(point),
        _first_segment_id(first_segment_id),
        _second_segment_id(second_segment_id) {
    }
    /**
     * @brief Возвращает точку пересечения только для чтения.
     *
     * @return const Point&
     */
    const Point& point() const {
        return _point;
    }

    /**
     * @brief Возвращает id первого отрезка.
     *
     * @return segment_id id первого отрезка.
     */
    segment_id first() const {
        return _first_segment_id;
    }

    /**
     * @brief Возвращает id второго отрезка.
     *
     * @return segment_id id второго отрезка.
     */
    segment_id second() const {
        return _second_segment_id;
    }

private:
    Point _point;
    segment_id _first_segment_id;
    segment_id _second_segment_id;
};

/**
 * @brief Класс для поиска пересечений отрезков.
 *
 */
class Intersection
{
public:
    Intersection() = delete;
    /**
     * @brief Ищет пересечение пары отрезков.
     *
     * @param first Первый отрезок.
     * @param second Второй отрезок.
     * @return Point Точка пересечения отрезков.
     */
    static Point intersectSegments(const Segment& first, const Segment& second);

    /**
     * @brief Ищет пересечения набора отрезков.
     *
     * @param segments Набор отрезков, в котором ищутся пересечения.
     * @return std::vector<IntersectionPoint> Перечень точект пересечений набора отрезков.
     */
    static std::vector<IntersectionPoint> intersectSetSegments(const SegmentsSet& segments);

private:
    enum event_status
    {
        start = 4,
        intersection_right = 3,
        intersection_left = 2,
        vertical = 1,
        end = 0
    };

    struct Event
    {
        Event(const gkernel::Segment& segment, gkernel::data_type x, event_status status): segment(&segment), x(x), status(status) {}

        bool operator<(const Event& other) const {
            if (std::abs(x - other.x) > 3 * EPS)
                return x < other.x;
            if (status != other.status)
                return static_cast<int8_t>(status) > static_cast<int8_t>(other.status);
            return this->segment->id < other.segment->id;
        }
        const Segment* segment;
        gkernel::data_type x;
        event_status status;
    };
};

} // namespace gkernel
#endif // __GKERNEL_HPP_INTERSECTION
