#ifndef __GKERNEL_HPP_OBJECTS
#define __GKERNEL_HPP_OBJECTS
#include "helpers.hpp"

namespace gkernel
{

    /**
     * @brief Структура, описывающая геометрический объект "точка" в двумерном пространстве.
     *
     */
    struct Point
    {
        /**
         * @brief Создаёт новый объект Point.
         *
         */
        Point() : _x(max_data_type_value), _y(max_data_type_value){};
        /**
         * @brief Создаёт новый объект Point.
         *
         * @param x x-координата
         * @param y y-координата
         */
        Point(data_type x, data_type y) : _x(x), _y(y) {}

        /**
         * @brief Оператор equal-to, проверяет равенство значений x, y this и other.
         *
         * @param other Объект Point, с которым происходит сравнение.
         * @return true this.x==other.x и this.y == other.y
         * @return false в противном случае
         */

        bool operator==(const Point &other) const
        {
            return this->_x == other._x && this->_y == other._y;
        }
        /**
         * @brief Оператор not-equal-to, проверяет равенство значений x, y this и other.
         *
         * @param other Объект Point, с которым происходит сравнение.
         * @return true this.x!=other.x или this.y != other.y
         * @return false this.x==other.x и this.y == other.y
         */
        bool operator!=(const Point &other) const
        {
            return !(*this == other);
        }

        /**
         * @brief Возвращает значение координаты x.
         *
         * @return data_type значение координаты x
         */
        data_type x() const { return _x; }
        /**
         * @brief Возвращает значение координаты y.
         *
         * @return data_type значение координаты y
         */
        data_type y() const { return _y; }

    private:
        data_type _x, _y;
    };

    /**
     * @brief Структура, описывающая геометрический объект "отрезок" в двумерном пространстве.
     *
     */
    struct Segment
    {
        /**
         * @brief Создаёт новый объект Segment.
         *
         */
        Segment() : _begin_point(), _end_point(){};
        /**
         * @brief Создаёт новый объект Segment.
         *
         * @param start начало отрезка
         * @param end конец отрезка
         */
        Segment(const Point &start, const Point &end) : _begin_point(start), _end_point(end) {}

        /**
         * @brief Оператор equal-to, проверяет равенство значений Point begin_point и Point end_point this и other.
         *
         * @param other Объект Segment, с которым происходит сравнение.
         * @return true this.begin_point == other.begin_point и this.end_point == other.end_point
         * @return false в противном случае
         */
        bool operator==(const Segment &other) const
        {
            return this->_begin_point == other._begin_point && this->_end_point == other._end_point;
        }

        /**
         * @brief Оператор equal-to, проверяет равенство значений Point begin_point и Point end_point this и other.
         *
         * @param other Объект Segment, с которым происходит сравнение.
         * @return true this.begin_point != other.begin_point или this.end_point != other.end_point
         * @return false this.begin_point == other.begin_point и this.end_point == other.end_point
         */
        bool operator!=(const Segment &other) const
        {
            return !(*this == other);
        }

        /**
         * @brief Возвращает объект Point, являющийся точкой начала отрезка.
         *
         * @return const Point& начало отрезка
         */
        const Point &start() const { return _begin_point; }

        /**
         * @brief Возвращает объект Point, являющийся точкой конца отрезка.
         *
         * @return const Point& конец отрезка
         */
        const Point &end() const { return _end_point; }

    private:
        Point _begin_point, _end_point;
    };

} // namespace gkernel
#endif // __GKERNEL_HPP_OBJECTS
