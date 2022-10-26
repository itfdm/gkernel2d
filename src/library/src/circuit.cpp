#include "gkernel/circuit.h"

using namespace gkernel;

Circuit::Circuit(const identifier& identifier)
    : m_id(identifier),
      m_leftmost_point{data_type_max, 0},
      m_rightmost_point{data_type_min, 0},
      m_lowest_point{0, data_type_max},
      m_highest_point{0, data_type_min} {}

void Circuit::push(const std::vector<Point>& points) {
    m_vertices.insert(m_vertices.end(), points.begin(), points.end());
    updateExtremePoints(points.size());
}

void Circuit::push(const Point& point) {
    m_vertices.emplace_back(point);
    updateExtremePoints(1);
}

void Circuit::updateExtremePoints(size_t num_added_points) {
    for (auto vertex_it = m_vertices.end() - num_added_points;
         vertex_it != m_vertices.end(); ++vertex_it) {
        m_leftmost_point =
            vertex_it->x < m_leftmost_point.x ? *vertex_it : m_leftmost_point;
        m_rightmost_point =
            vertex_it->x > m_rightmost_point.x ? *vertex_it : m_rightmost_point;
        m_lowest_point =
            vertex_it->y < m_lowest_point.y ? *vertex_it : m_lowest_point;
        m_highest_point =
            vertex_it->y > m_highest_point.y ? *vertex_it : m_highest_point;
    }
}
