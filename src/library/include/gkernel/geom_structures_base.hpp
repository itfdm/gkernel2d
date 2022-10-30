#include <vector>
#include <limits>
#include <stdexcept>
#include <algorithm>

namespace gkernel {

using data_type = uint64_t;
using label_data_type = uint64_t;
using label_type = uint8_t;

constexpr data_type min_data_type_value = std::numeric_limits<data_type>::min();
constexpr data_type max_data_type_value = std::numeric_limits<data_type>::max();


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
    Segment() : _begin_point(), _end_point() {};
    Segment(const Point& begin_point, const Point& end_point) : _begin_point(begin_point), _end_point(end_point) {}

    bool operator==(const Segment& other) const {
        return this->_begin_point == other._begin_point && this->_end_point == other._end_point;
    }
    bool operator!=(const Segment& other) const {
        return !(*this == other);
    }

    Point begin_point() const { return _begin_point; }
    Point end_point() const { return _end_point; }

private:
    Point _begin_point, _end_point;
};

class Labeling {
protected:
    Labeling() : _label_types({}), _labels_data({}), _segments_size(0) {}
public:
    bool has_label(label_type label) const {
        return std::find(_label_types.begin(), _label_types.end(), label) != _label_types.end();
    }

    const std::vector<label_type>& get_label_types() const {
        return _label_types;
    }

    label_data_type get_label_value(label_type label, size_t segment_idx) const {
        return _labels_data[get_offset(label) + segment_idx];
    }

    void set_labels_types(const std::vector<label_type>& label_types) {
        if (_segments_size == 0) {
            throw std::runtime_error("Segments not initialized.");
        }
        _label_types = label_types;
        _labels_data.resize(label_types.size() * _segments_size);
    }

    void set_label_values(label_type label, const std::vector<label_data_type>& label_data) {
        if (label_data.size() != _segments_size) {
            throw std::runtime_error("The number of label data does not match the number of segments.");
        }
        std::copy(label_data.begin(), label_data.end(), _labels_data.begin() + get_offset(label));
    }

    void set_label_value(label_type label, size_t segment_idx, label_data_type label_value) {
        _labels_data[get_offset(label) + segment_idx] = label_value;
    }

protected:
    void update_segments_size(size_t segments_size) {
        _segments_size = segments_size;
    }
    std::vector<label_type> _label_types;

private:
    size_t get_offset(label_type label) const {
        if (!has_label(label)) {
            throw std::runtime_error("Label not found.");
        }
        size_t label_idx = std::distance(_label_types.begin(),
                                         std::find(_label_types.begin(), _label_types.end(), label));
        return label_idx * _segments_size;
    }

    size_t _segments_size;
    std::vector<label_data_type> _labels_data; // store like (s1_l1, s2_l1 .. sn_l1, s1_l2 .. sn_l2, .., s1_lk .. sn_lk)
};

class SegmentsSetCommon : public Labeling {
protected:
    SegmentsSetCommon() : _segments({}) {}
    SegmentsSetCommon(const std::vector<Segment>& segments) : _segments(segments.begin(), segments.end()) {
        update_segments_size(segments.size());
    }

public:
    bool operator==(const SegmentsSetCommon& other) const {
       return std::equal(this->_segments.begin(), this->_segments.end(), other._segments.begin());
    }
    bool operator!=(const SegmentsSetCommon& other) const {
        return !(*this == other);
    }

    const Segment& get_segment(size_t idx) const {
        return _segments.at(idx);
    }
    size_t size() const {
        return _segments.size();
    }

protected:
    std::vector<Segment> _segments;

    friend class CircuitsSet;
};

class SegmentsSet : public SegmentsSetCommon {
public:
    SegmentsSet() : SegmentsSetCommon() {}
    SegmentsSet(const SegmentsSet& segment_set) : SegmentsSetCommon(segment_set._segments) {}
    SegmentsSet(const std::vector<Segment>& segments) : SegmentsSetCommon(segments) {}

    virtual void emplace_back(const Segment& segment) {
        if (!_label_types.empty()) {
            throw std::runtime_error("Unable to add elements after labels initialization.");
        }
        _segments.emplace_back(segment);
        update_segments_size(_segments.size());
    }
};

using SegmentsLayer = const SegmentsSet;

class Validator {
protected:
    Validator(bool validation = true) : _validation(validation) {}

    void set_validation(bool validation) {
        _validation = validation;
    }

    virtual void validate() const = 0;

    bool _validation;
};

class VertexChain : public SegmentsSet, public Validator {
public:
    VertexChain() : SegmentsSet() {}
    VertexChain(const std::vector<Segment>& segments, bool validation = true) : SegmentsSet(segments), Validator(validation) {
        if (_validation) {
            validate();
        }
    }

    void emplace_back(const Segment& segment) override {
        if (_validation && !_segments.empty()) {
            if (_segments.back().end_point() != segment.begin_point()) {
                throw std::runtime_error("Validation failed. Invalid emplaced segment.");
            }
        }
        SegmentsSet::emplace_back(segment);
    }

private:
    void validate() const override {
        for (size_t i = 0; i < _segments.size() - 1; ++i) {
            if (_segments[i].end_point() != _segments[i + 1].begin_point()) {
                throw std::runtime_error("Validation failed. Segment set is not a chain.");
            }
        }
    }
};

class Circuit : public SegmentsSetCommon, public Validator {
public:
    Circuit() : SegmentsSetCommon() {}
    Circuit(const Circuit& circuit) : SegmentsSetCommon(circuit._segments) {}
    Circuit(const std::vector<Segment>& segments, bool validation = true) : SegmentsSetCommon(segments), Validator(validation) {
        if (_validation) {
            validate();
        }
    }

private:
    void validate() const override {
        for (size_t i = 0; i < _segments.size(); ++i) {
            if (_segments[i].end_point() != _segments[(i + 1) % _segments.size()].begin_point()) {
                throw std::runtime_error("Validation failed. Segment set is not a circuit.");
            }
        }
    }
};

class CircuitsSet : public Labeling {
public:
    CircuitsSet() : _segments({}), _indices({}) {}
    CircuitsSet(const CircuitsSet& circuits) : _segments(circuits._segments.begin(), circuits._segments.end()),
                                               _indices(circuits._indices.begin(), circuits._indices.end()) {
        update_segments_size(_segments.size());
    }
    CircuitsSet(const std::vector<Circuit>& circuits) {
        _indices = {0};
        for (const auto& circuit : circuits) {
            _segments.insert(_segments.end(), circuit._segments.begin(), circuit._segments.end());
            _indices.emplace_back(_segments.size());
        }
        update_segments_size(_segments.size());
    }

    Circuit get_circuit(size_t idx) const {
        std::vector<Segment> segments(_segments.begin() + _indices[idx], _segments.begin() + _indices[idx + 1]);
        return Circuit(segments, false);
    }

    void emplace_back(const Circuit& circuit) {
        if (!_label_types.empty()) {
            throw std::runtime_error("Unable to add elements after labels initialization.");
        }
        _segments.insert(_segments.end(), circuit._segments.begin(), circuit._segments.end());
        _indices.emplace_back(_segments.size());
        update_segments_size(_segments.size());
    }

    size_t size() const {
        return _indices.size() - 1;
    }

private:
    std::vector<Segment> _segments;
    std::vector<size_t> _indices; // circuit bounds (ex: if input circuits has size eq 2, 3, 4 respectively, then _indices={0, 2, 5, 9})
};

using CircuitsLayer = const CircuitsSet;

} // namespace gkernel
