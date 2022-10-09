#ifndef __GKERNEL_H_HELPERS
#define __GKERNEL_H_HELPERS

#include <iostream>
#include <string>

namespace gkernel {

/**
 * @brief Тип данных, используемый для представления координат точки.
 *
 */
using data_type = long long;

/**
 * @brief Максимальное значение data_type.
 *
 */
constexpr data_type data_type_max = std::numeric_limits<data_type>::max();

/**
 * @brief Минимальное значение data_type.
 *
 */
constexpr data_type data_type_min = std::numeric_limits<data_type>::min();

constexpr unsigned int resolution_height = 768;
constexpr unsigned int resolution_width = 1366;

/**
 * @brief Условия сравнения.
 *
 */
enum class Sign { EQUAL, GE, LE };

/**
 * @brief Структура, представляющая описание точки контура.
 *
 */
struct Point {
    data_type x;
    data_type y;

    /**
     * @brief Оператор равенства точек.
     *
     * @param p сравниваемая точка
     */
    bool operator==(const Point& p) const {
        return this->x == p.x && this->y == p.y;
    }

    /**
     * @brief Оператор неравенства точек.
     *
     * @param p сравниваемая точка
     */
    bool operator!=(const Point& p) const {
        return !(*this == p);
    }
};

/**
 * @brief Перегрузка оператора для вывода текстового представления Point в поток вывода.
 *
 * @param os поток вывода
 * @param point выводимая точка
 * @return std::ostream&
 */
inline std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

/**
 * @brief Тип данных, используемый для представления ID контура.
 *
 */
using identifier = long long;

/**
 * @brief Идентификатор всей плоскости.
 *
 */
constexpr gkernel::identifier plane_id = -1;

}  // namespace gkernel
#endif /* __GKERNEL_H_HELPERS */
