#include "gkernel/serializer.hpp"
#include <iostream>

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

    std::vector<Segment> segments_to_save;
    segments_to_save.reserve(segments_set.size());
    for (std::size_t idx = 0; idx < segments_set.size(); ++idx) {
        segments_to_save.push_back(segments_set[idx]);
    }

    std::sort(segments_to_save.begin(), segments_to_save.end(), [](Segment& lhs, Segment& rhs) {
        return lhs.get_id() < rhs.get_id();
    });

    for (std::size_t idx = 0; idx < segments_to_save.size(); ++idx) {
        const auto& segment = segments_to_save[idx];
        file << segment.start().x() << " " << segment.start().y() << " " << segment.end().x() << " " << segment.end().y();
        if (idx != segments_to_save.size() - 1) {
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
            file << segment.start().x() << " " << segment.start().y() << " " << segment.end().x() << " " << segment.end().y();
            if (idx != segments_set.size() - 1) {
                file << " ";
            }
        }
        file << std::endl;
    }
}
