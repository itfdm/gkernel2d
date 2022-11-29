#ifndef __GKERNEL_HPP_OBJECTS
#define __GKERNEL_HPP_OBJECTS
#include "helpers.hpp"
#include <cmath>
#include <iostream>

namespace gkernel {

struct Point {
    Point() : _x(max_data_type_value), _y(max_data_type_value) {};
    Point(data_type x, data_type y) : _x(x), _y(y) {}

    bool operator==(const Point& other) const {
        return this->_x == other._x && this->_y == other._y;
    }
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    data_type x() const { return _x; }
    data_type y() const { return _y; }

private:
    data_type _x, _y;
};

struct Segment {
    Segment() : _begin_point(), _end_point() {
        id = get_id();
    }
    Segment(const Point& start, const Point& end) : _begin_point(start), _end_point(end) {
        id = get_id();
    }

    bool operator==(const Segment& other) const {
        return this->_begin_point == other._begin_point && this->_end_point == other._end_point;
    }
    bool operator!=(const Segment& other) const {
        return !(*this == other);
    }

    const Point& start() const { return _begin_point; }
    const Point& end() const { return _end_point; }

    std::size_t get_id() {
        static std::size_t idx = 0;
        return idx++;
    }
    std::size_t hash() const {
        return std::hash<data_type>()(_begin_point.x()) ^ std::hash<data_type>()(_begin_point.y()) ^
            std::hash<data_type>()(_end_point.x()) ^ std::hash<data_type>()(_end_point.y());
    }
    std::size_t id;
private:
    Point _begin_point, _end_point;
};

} // namespace gkernel
#endif // __GKERNEL_HPP_OBJECTS
