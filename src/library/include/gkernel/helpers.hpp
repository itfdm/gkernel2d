#ifndef __GKERNEL_HPP_HELPERS
#define __GKERNEL_HPP_HELPERS

#define NOMINMAX

#include <cmath>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <iostream>

namespace gkernel {

struct Segment;

using data_type = double;
using label_data_type = int64_t;
using label_type = uint8_t;
using segment_id = size_t;

constexpr data_type min_data_type_value = std::numeric_limits<data_type>::min();
constexpr data_type max_data_type_value = std::numeric_limits<data_type>::max();

class Labeling {
protected:
    Labeling() : _label_types({}), _labels_data({}) {}
public:
    bool has_label(label_type label) const {
        return std::find(_label_types.begin(), _label_types.end(), label) != _label_types.end();
    }

    const std::vector<label_type>& get_label_types() const {
        return _label_types;
    }

    virtual label_data_type get_label_value(label_type label, const Segment& segment) const = 0;

    virtual std::vector<label_data_type>& get_label_values(label_type label) = 0;

    virtual void set_labels_types(const std::vector<label_type>& label_types) = 0;

    virtual void set_label_values(label_type label, const std::vector<label_data_type>& label_data) = 0;

    virtual void set_label_value(label_type label, const Segment& segment, label_data_type label_value) = 0;

protected:
    std::vector<label_type> _label_types;
    std::vector<std::vector<label_data_type>> _labels_data;
};


class Validator {
protected:
    Validator(bool validation = true) : _validation(validation) {}

    void set_validation(bool validation) {
        _validation = validation;
    }

    virtual void validate() const = 0;

    bool _validation;
};

static inline void throw_exception(std::string message) {
    std::cerr << message << std::endl;
    throw std::runtime_error(message);
}

} // namespace gkernel
#endif /* __GKERNEL_HPP_HELPERS */
