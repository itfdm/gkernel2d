#ifndef __GKERNEL_H_CIRCUIT
#define __GKERNEL_H_CIRCUIT

#include <vector>

#include "helpers.h"

namespace gkernel {

/**
	Класс, представляющий описание контура.
*/
class Circuit {
public:
    /**
     * @brief Создает новый экземпляр класса Circuit.
     *
     * @param identifier ID контура
     */
    Circuit(const identifier& identifier);

    /**
     * @brief Добавляет точку в контур.
     *
     * @param point добавляемая точка
     */
    void push(const Point& point);

    void pop(){
        m_vertices.pop_back();
    }

    /**
     * @brief Добавляет множество точек в контур.
     *
     * @param points добавляемые точки
     */
    void push(const std::vector<Point>& points);

    /**
     * @brief Меняет направление обхода контура.
     *
     */
    void reverse() { std::reverse(m_vertices.begin(), m_vertices.end()); }

    /**
     * @brief Возвращает итератор на первую точку контура.
     *
     * @return std::vector<Point>::iterator
     */
    std::vector<Point>::iterator begin() { return m_vertices.begin(); }

    /**
     * @brief Возвращает итератор на точку, следующую за последней точкой.
     *
     * @return std::vector<Point>::iterator
     */
    std::vector<Point>::iterator end() { return m_vertices.end(); }

    /**
     * @brief Заменяет точку по указанному индексу на переданную.
     *
     * @param idx индекс заменяемой точки
     * @param p новая точка
     */
    void editPoint(size_t idx, const Point& p) {
        m_vertices[idx] = p;
        updateExtremePoints(m_vertices.size());
    }

    /**
     * @brief Возвращает количество вершин в контуре (включая точку замыкания).
     *
     * @return size_t
     */
    size_t size() const { return m_vertices.size(); }

    /**
     * @brief Возврашает ID контура.
     *
     * @return identifier
     */
    identifier getID() const { return m_id; }

    /**
     * @brief Возвращает точку контура с наименьшим значением координаты x.
     *
     * @return const Point&
     */
    const Point& getLeftmostPoint() const { return m_leftmost_point; }

    /**
     * @brief Возвращает точку контура с наибольшим значением координаты x.
     *
     * @return const Point&
     */
    const Point& getRightmostPoint() const { return m_rightmost_point; }

    /**
     * @brief Возвращает точку контура с наименьшим значением координаты y.
     *
     * @return const Point&
     */
    const Point& getLowestPoint() const { return m_lowest_point; }

    /**
     * @brief Возвращает точку контура с наибольшим значением координаты y.
     *
     * @return const Point&
     */
    const Point& getHighestPoint() const { return m_highest_point; }

    /**
     * @brief Предоставляет доступ (read-only) к точке с указанным индексом.
     *
     * @param idx индекс точки
     * @return const Point&
     */
    const Point& operator[](std::size_t idx) const {
        return m_vertices[idx];
    }

    /**
     * @brief Разрушает данный экземпляр класса Circuit.
     *
     */
    ~Circuit() = default;

private:
    void updateExtremePoints(size_t num_added_points);

    std::vector<Point> m_vertices{};
    identifier m_id;
    Point m_leftmost_point;
    Point m_rightmost_point;
    Point m_lowest_point;
    Point m_highest_point;
};

/**
 * @brief Перегрузка оператора для вывода текстового представления Circuit в поток вывода.
 *
 * @param os поток вывода
 * @param circuit выводимый контур
 * @return std::ostream&
 */
inline std::ostream& operator<<(std::ostream& os, const Circuit& circuit) {
    os << "[";
    for (size_t i = 0; i < circuit.size(); ++i) {
        os << circuit[i];
        os << ((i != circuit.size() - 1) ? ", " : "]");
    }
    return os;
}

}  // namespace gkernel
#endif /* __GKERNEL_H_CIRCUIT */
