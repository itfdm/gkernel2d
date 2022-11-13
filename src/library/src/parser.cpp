#include "gkernel/parser.hpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace gkernel;

SegmentsSet FileParser::parseSegmentsSet(const std::string& path) {
    std::ifstream input(path, std::ifstream::in);
    SegmentsSet segments;
    if (!input.is_open()) {
        std::string error_message = "Cannot open file " + path + "\n";
#if GKERNEL_DEBUG
        std::cerr << error_message << std::endl;
#endif
        throw std::runtime_error(error_message);
    }

    while (input.peek() != EOF) {
        data_type x1, y1, x2, y2;
        input >> x1 >> y1 >> x2 >> y2;
        segments.emplace_back(Segment(Point(x1, y1), Point(x2, y2)));
    }

    input.close();
    return segments;
}

// parse vector of segments set from file with format: every line is a set of segments
std::vector<SegmentsSet> FileParser::parseVectorOfSegmentsSet(const std::string& path) {
    std::ifstream input(path, std::ifstream::in);
    std::vector<SegmentsSet> segments_sets;
    if (!input.is_open()) {
        std::string error_message = "Cannot open file " + path + "\n";
#if GKERNEL_DEBUG
        std::cerr << error_message << std::endl;
#endif
        throw std::runtime_error(error_message);
    }

    while (input.peek() != EOF) {
        SegmentsSet segments;
        while (input.peek() != '\n' && input.peek() != EOF) {
            data_type x1, y1, x2, y2;
            input >> x1 >> y1 >> x2 >> y2;
            segments.emplace_back(Segment(Point(x1, y1), Point(x2, y2)));
            // ignore Windows line endings
            if (input.peek() == '\r') {
                input.get();
            }
        }
        segments_sets.emplace_back(std::move(segments));
        input.get();
    }

    input.close();
    return segments_sets;
}
