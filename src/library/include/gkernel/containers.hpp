#ifndef __GKERNEL_HPP_CONTAINERS
#define __GKERNEL_HPP_CONTAINERS

#include "objects.hpp"

namespace gkernel {

class SegmentsSetCommon : public Labeling {
protected:
    SegmentsSetCommon() : _segments({}) {}
    SegmentsSetCommon(const std::vector<Segment>& segments) : _segments(segments.begin(), segments.end()) {
        for (size_t i = 0; i < _segments.size(); ++i) {
            _segments[i].id = i;
        }
    }
    SegmentsSetCommon(std::vector<Segment>&& segments) : _segments(std::move(segments)) {
        for (size_t i = 0; i < _segments.size(); ++i) {
            _segments[i].id = i;
        }
    }

public:
    bool operator==(const SegmentsSetCommon& other) const {
       return std::equal(this->_segments.begin(), this->_segments.end(), other._segments.begin());
    }
    bool operator!=(const SegmentsSetCommon& other) const {
        return !(*this == other);
    }

    label_data_type get_label_value(label_type label, const Segment& segment) const override {
        return _labels_data[get_offset(label) + segment.id];
    }

    void set_labels_types(const std::vector<label_type>& label_types) override;

    void set_label_values(label_type label, const std::vector<label_data_type>& label_data) override;

    void set_label_value(label_type label, const Segment& segment, label_data_type label_value) override {
        _labels_data[get_offset(label) + segment.id] = label_value;
    }
    const Segment& operator[](size_t idx) const {
        return _segments[idx];
    }
    size_t size() const {
        return _segments.size();
    }

private:
    size_t get_offset(label_type label) const;

protected:
    std::vector<Segment> _segments;

    friend class CircuitsSet;
};

class SegmentsSet : public SegmentsSetCommon {
public:
    SegmentsSet() : SegmentsSetCommon() {}
    SegmentsSet(const std::vector<Segment>& segments) : SegmentsSetCommon(segments) {}

    virtual void emplace_back(const Segment& segment) {
        if (!_label_types.empty()) {
            throw std::runtime_error("Unable to add elements after labels initialization.");
        }
        _segments.emplace_back(segment);
        _segments.back().id = _segments.size() - 1;
    }
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
            if (_segments.back().end() != segment.start()) {
                throw std::runtime_error("Validation failed. Invalid emplaced segment.");
            }
        }
        SegmentsSet::emplace_back(segment);
    }

private:
    void validate() const override {
        for (size_t i = 0; i < _segments.size() - 1; ++i) {
            if (_segments[i].end() != _segments[i + 1].start()) {
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
            if (_segments[i].end() != _segments[(i + 1) % _segments.size()].start()) {
                throw std::runtime_error("Validation failed. Segment set is not a circuit.");
            }
        }
    }
};

class CircuitsSet : public Labeling {
public:
    CircuitsSet() : _segments({}), _indices({}) {}
    CircuitsSet(const CircuitsSet& circuits) : _segments(circuits._segments.begin(), circuits._segments.end()),
                                               _indices(circuits._indices.begin(), circuits._indices.end()) {}
    CircuitsSet(const std::vector<Circuit>& circuits) {
        _indices = {0};
        for (const auto& circuit : circuits) {
            _segments.insert(_segments.end(), circuit._segments.begin(), circuit._segments.end());
            _indices.emplace_back(_segments.size());
        }
    }

    label_data_type get_label_value(label_type label, const Segment& segment) const override { return 0; /*TODO*/};
    void set_labels_types(const std::vector<label_type>& label_types) override {/*TODO*/};
    void set_label_values(label_type label, const std::vector<label_data_type>& label_data) override {/*TODO*/};
    void set_label_value(label_type label, const Segment& segment, label_data_type label_value) override {/*TODO*/};

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
    }

    size_t size() const {
        return _indices.size() - 1;
    }

private:
    std::vector<Segment> _segments;
    std::vector<size_t> _indices; // circuit bounds (ex: if input circuits have size eq 2, 3, 4 respectively, then _indices={0, 2, 5, 9})
};

using SegmentsLayer = const SegmentsSet;
using CircuitsLayer = const CircuitsSet;

} // namespace gkernel
#endif /* __GKERNEL_HPP_CONTAINERS */
