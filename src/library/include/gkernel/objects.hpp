#ifndef __GKERNEL_HPP_OBJECTS
#define __GKERNEL_HPP_OBJECTS

#include "helpers.hpp"
#include <iostream>

namespace gkernel {

class SegmentsSetCommon;
class SegmentsSet;
class Intersection;
class Converter;
class AreaAnalyzer;

/**
 * @brief Структура, описывающая геометрический объект "точка" в двумерном пространстве.
 *
 */
struct Point
{
    /**
     * @brief Конструктор структуры.
     *
     */
    Point(): _x(max_data_type_value), _y(max_data_type_value) {};
    /**
     * @brief Конструктор структуры.
     *
     * @param x x-координата
     * @param y y-координата
     */
    Point(data_type x, data_type y): _x(x), _y(y) {}

    /**
     * @brief Оператор equal-to, проверяет равенство значений x, y this и other.
     *
     * @param other Объект Point, с которым происходит сравнение.
     * @return true this.x==other.x и this.y == other.y
     * @return false в противном случае
     */
    bool operator==(const Point& other) const {
        return this->_x == other._x && this->_y == other._y;
    }

    /**
     * @brief Оператор not-equal-to, проверяет равенство значений x, y this и other.
     *
     * @param other Объект Point, с которым происходит сравнение.
     * @return true this.x!=other.x или this.y != other.y
     * @return false this.x==other.x и this.y == other.y
     */
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    /**
     * @brief Оператор сравнения "меньше".
     *
     * @param other Второй операнд.
     * @return true если this->_x < other._x || this->_x == other._x && this->_y < other._y
     * @return false иначе.
     */
    bool operator<(const Point& other) const {
        return this->_x < other._x ||
            (this->_x == other._x && this->_y < other._y);
    }

    /**
     * @brief Оператор сравнения "больше".
     *
     * @param other Второй операнд.
     * @return true если this->_x > other._x || this->_x == other._x && this->_y > other._y
     * @return false иначе.
     */
    bool operator>(const Point& other) const {
        return this->_x > other._x ||
            (this->_x == other._x && this->_y > other._y);
    }

    /**
     * @brief Возвращает значение координаты x.
     *
     * @return data_type Значение координаты x.
     */
    data_type x() const { return _x; }

    /**
     * @brief Возвращает значение координаты y.
     *
     * @return data_type Значение координаты y.
     */
    data_type y() const { return _y; }

private:
    data_type _x, _y;
};

inline std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.x() << ", " << point.y() << ")";
    return os;
}

/**
 * @brief Структура, описывающая геометрический объект "отрезок" в двумерном пространстве.
 *
 */
struct Segment
{
    /**
     * @brief Конструктор структуры.
     *
     */
    Segment(): _begin_point(), _end_point() {};

    /**
     * @brief Конструктор структуры.
     *
     * @param start Начало отрезка.
     * @param end Конец отрезка.
     */
    Segment(const Point& start, const Point& end): _begin_point(start), _end_point(end) {}

    /**
     * @brief Проверяет совпадение координат концевых точек отрезка.
     *
     * @return true Координаты совпадают.
     * @return false Координаты не совпадают.
     */
    bool is_point() const {
        return _begin_point == _end_point;
    }

    /**
     * @brief Оператор equal-to, проверяет равенство значений Point begin_point и Point end_point this и other.
     *
     * @param other Объект Segment, с которым происходит сравнение.
     * @return true this.begin_point == other.begin_point и this.end_point == other.end_point
     * @return false в противном случае
     */
    bool operator==(const Segment& other) const {
        return (this->_begin_point == other._begin_point && this->_end_point == other._end_point) ||
            (this->_begin_point == other._end_point && this->_end_point == other._begin_point);
    }

    /**
     * @brief Оператор equal-to, проверяет равенство значений Point begin_point и Point end_point this и other.
     *
     * @param other Объект Segment, с которым происходит сравнение.
     * @return true this.begin_point != other.begin_point или this.end_point != other.end_point
     * @return false this.begin_point == other.begin_point и this.end_point == other.end_point
     */
    bool operator!=(const Segment& other) const {
        return !(*this == other);
    }

    /**
     * @brief Возвращает точку начала отрезка.
     *
     * @return const Point& Начало отрезка.
     */
    const Point& start() const { return _begin_point; }

    /**
     * @brief Возвращает точку конца отрезка.
     *
     * @return const Point& Конец отрезка.
     */
    const Point& end() const { return _end_point; }

    /**
     * @brief Сравнивает точку начала и конца отрезка и возвращает ту, что меньше в соответствии с оператором "меньше" для объектов Point.
     *
     * @return const Point& Меньшая из двух концевых точек отрезка.
     */
    const Point& min() const {
        return _begin_point < _end_point ? _begin_point : _end_point;
    }

    /**
     * @brief Сравнивает точку начала и конца отрезка и возвращает ту, что больше в соответствии с оператором "больше" для объектов Point.
     *
     * @return const Point& Большая из двух концевых точек отрезка.
     */
    const Point& max() const {
        return _begin_point > _end_point ? _begin_point : _end_point;
    }

private:
    Point _begin_point, _end_point;
    segment_id id;

    friend class SegmentsSetCommon;
    friend class SegmentsSet;
    friend class Intersection;
    friend class Converter;
    friend class AreaAnalyzer;
};

inline std::ostream& operator<<(std::ostream& os, const Segment& segment) {
    os << "[" << segment.start() << ", " << segment.end() << "]";
    return os;
}

} // namespace gkernel
#endif // __GKERNEL_HPP_OBJECTS
