#ifndef __GKERNEL_HPP_HELPERS
#define __GKERNEL_HPP_HELPERS

#define NOMINMAX

#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>

namespace gkernel {

struct Segment;

using data_type = double;
using label_data_type = int64_t;
using label_type = uint8_t;
using segment_id = size_t;

constexpr data_type min_data_type_value = std::numeric_limits<data_type>::min();
constexpr data_type max_data_type_value = std::numeric_limits<data_type>::max();

/**
 * @brief Контейнер для хранения типов меток и их значений.
 *
 */
class Labeling
{
protected:
    Labeling(): _label_types({}), _labels_data({}) {}

public:
    /**
     * @brief Возвращает true, если для контейнера задан тип метки label, иначе false.
     *
     * @param label Тип метки.
     * @return true Если тип метки label есть в контейнере.
     * @return false Если тип метки label отсутствует в контейнере.
     */
    bool has_label(label_type label) const {
        return std::find(_label_types.begin(), _label_types.end(), label) != _label_types.end();
    }

    /**
     * @brief Предоставляет типы меток, хранящиеся в контейнере.
     *
     * @return const std::vector<label_type>& Типы меток, хранящиеся в контейнере.
     */
    const std::vector<label_type>& get_label_types() const {
        return _label_types;
    }

    /**
     * @brief Возвращает значение метки для указанного отрезка.
     *
     * @param label Тип метки.
     * @param segment Отрезок, значение метки которого хотим узнать.
     * @return label_data_type Значение метки.
     */
    virtual label_data_type get_label_value(label_type label, const Segment& segment) const = 0;

    virtual std::vector<label_data_type> get_label_values(label_type label) const = 0;

    /**
     * @brief Устанавливает типы меток.
     *
     * @param label_types Набор типов меток.
     */
    virtual void set_labels_types(const std::vector<label_type>& label_types) = 0;

    /**
     * @brief Устанавливает значение метки указанного типа для каждого объекта в контейнере.
     *
     * @param label Тип метки.
     * @param label_data Значение метки для объекта с индексом idx = label_data[idx]
     */
    virtual void set_label_values(label_type label, const std::vector<label_data_type>& label_data) = 0;

    /**
     * @brief Устанавливает значение метки указанного типа для заданного отрезка.
     *
     * @param label Тип метки.
     * @param segment Отрезок, для которого устанавливается значение метки.
     * @param label_value Значение метки.
     */
    virtual void set_label_value(label_type label, const Segment& segment, label_data_type label_value) = 0;

protected:
    std::vector<label_type> _label_types;
    std::vector<label_data_type> _labels_data; // store like (s1_l1, s2_l1 .. sn_l1, s1_l2 .. sn_l2, .., s1_lk .. sn_lk)
};

/**
 * @brief Родительский класс для контейнеров отрезков, добавляющий возможность определять функцию валидации хранящихся данных.
 *
 */
class Validator
{
protected:
    Validator(bool validation = true): _validation(validation) {}

    void set_validation(bool validation) {
        _validation = validation;
    }

    /**
     * @brief Функция валидации.
     *
     */
    virtual void validate() const = 0;

    bool _validation;
};

} // namespace gkernel
#endif /* __GKERNEL_HPP_HELPERS */
