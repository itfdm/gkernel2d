#include "gkernel/containers.hpp"

namespace gkernel {

void SegmentsSetCommon::set_labels_types(const std::vector<label_type>& label_types) {
    if (_segments.size() == 0) {
        throw std::runtime_error("Segments not initialized.");
    }
    if (!_label_types.empty()) {
        throw std::runtime_error("Label types already initialized.");
    }
    _label_types = label_types;
    _labels_data.resize(_label_types.size());
    for (size_t i = 0; i < _label_types.size(); ++i) {
        _labels_data[i].resize(_segments.size());
    }
}

void SegmentsSetCommon::set_label_values(label_type label, const std::vector<label_data_type>& label_data) {
    if (label_data.size() != _segments.size()) {
        throw std::runtime_error("The number of label data does not match the number of segments.");
    }
    std::copy(label_data.begin(), label_data.end(), _labels_data[label].begin());
}

size_t SegmentsSetCommon::get_offset(label_type label) const {
#if GKERNEL_DEBUG
    if (!has_label(label)) {
        throw std::runtime_error("Label not found.");
    }
#endif
    size_t label_idx = std::distance(_label_types.begin(),
                                     std::find(_label_types.begin(), _label_types.end(), label));
    return label_idx * _segments.size();
}

} // namespace gkernel
