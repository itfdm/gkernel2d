#include "gkernel/serializer.hpp"

using namespace gkernel;

void OutputSerializer::serializeSegmentsSet(const SegmentsSet& segments_set, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::string error_message = "Cannot open file " + path + "\n";
#if GKERNEL_DEBUG
        std::cerr << error_message << std::endl;
#endif
        throw std::runtime_error(error_message);
    }

    for (std::size_t idx = 0; idx < segments_set.size(); ++idx) {
        const auto& segment = segments_set[idx];
        file << segment.begin_point().x() << " " << segment.begin_point().y() << " " << segment.end_point().x() << " " << segment.end_point().y();
        if (idx != segments_set.size() - 1) {
            file << " ";
        }
    }
}

void OutputSerializer::serializeVectorOfSegmentsSet(const std::vector<SegmentsSet>& vector_of_segments_set, const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::string error_message = "Cannot open file " + path + "\n";
#if GKERNEL_DEBUG
        std::cerr << error_message << std::endl;
#endif
        throw std::runtime_error(error_message);
    }

    for (const auto& segments_set : vector_of_segments_set) {
        for (std::size_t idx = 0; idx < segments_set.size(); ++idx) {
            const auto& segment = segments_set[idx];
            file << segment.begin_point().x() << " " << segment.begin_point().y() << " " << segment.end_point().x() << " " << segment.end_point().y();
            if (idx != segments_set.size() - 1) {
                file << " ";
            }
        }
        file << std::endl;
    }
}
