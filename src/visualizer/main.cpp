#define WITHOUT_NUMPY 1

#include <vector>

#include "gkernel/containers.hpp"
#include "gkernel/parser.hpp"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using axis_vector = std::vector<gkernel::data_type>;

constexpr std::size_t resolution_width = 1366;
constexpr std::size_t resolution_height = 768;

constexpr double point_marker_size = 7;

// gkernel::SegmentsSet get_segments() {
//     gkernel::SegmentsSet segvec;
//     segvec.emplace_back({gkernel::Point(9, 6), gkernel::Point(10, 5)});
//     segvec.emplace_back({gkernel::Point(6, 1), gkernel::Point(10, 5)});
//     segvec.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(4, 1)});
//     segvec.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(6, 3)});
//     segvec.emplace_back({gkernel::Point(4, 3), gkernel::Point(6, 1)});
//     segvec.emplace_back({gkernel::Point(4, 3), gkernel::Point(6, 5)});
//     segvec.emplace_back({gkernel::Point(4, 1), gkernel::Point(6, 3)});
//     segvec.emplace_back({gkernel::Point(6, 5), gkernel::Point(9, 6)});
//     segvec.emplace_back({gkernel::Point(6, 4), gkernel::Point(9, 5)});
//     segvec.emplace_back({gkernel::Point(6, 4), gkernel::Point(8, 0)});
//     segvec.emplace_back({gkernel::Point(9, 5), gkernel::Point(11, 7)});
//     segvec.emplace_back({gkernel::Point(8, 0), gkernel::Point(13, 5)});
//     segvec.emplace_back({gkernel::Point(10, 4), gkernel::Point(13, 7)});
//     segvec.emplace_back({gkernel::Point(11, 7), gkernel::Point(13, 5)});
//     segvec.emplace_back({gkernel::Point(10, 4), gkernel::Point(12, 2)});
//     segvec.emplace_back({gkernel::Point(11, 2), gkernel::Point(13, 4)});
//     segvec.emplace_back({gkernel::Point(11, 2), gkernel::Point(12, 1)});
//     segvec.emplace_back({gkernel::Point(12, 1), gkernel::Point(15, 2)});
//     segvec.emplace_back({gkernel::Point(12, 2), gkernel::Point(16, 4)});
//     segvec.emplace_back({gkernel::Point(13, 4), gkernel::Point(15, 2)});
//     segvec.emplace_back({gkernel::Point(13, 7), gkernel::Point(14, 5)});
//     segvec.emplace_back({gkernel::Point(14, 5), gkernel::Point(16, 4)});
//     return segvec;
// }

// gkernel::SegmentsSet get_results() {
//     gkernel::SegmentsSet expected;
//     expected.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(3.5, 5.5)});
//     expected.emplace_back({gkernel::Point(4, 3), gkernel::Point(4, 3)});
//     expected.emplace_back({gkernel::Point(4, 1), gkernel::Point(4, 1)});
//     expected.emplace_back({gkernel::Point(5, 4), gkernel::Point(5, 4)});
//     expected.emplace_back({gkernel::Point(5, 2), gkernel::Point(5, 2)});
//     expected.emplace_back({gkernel::Point(6, 5), gkernel::Point(6, 5)});
//     expected.emplace_back({gkernel::Point(6, 4), gkernel::Point(6, 4)});
//     expected.emplace_back({gkernel::Point(6, 3), gkernel::Point(6, 3)});
//     expected.emplace_back({gkernel::Point(6, 1), gkernel::Point(6, 1)});
//     expected.emplace_back({gkernel::Point(7, 2), gkernel::Point(7, 2)});
//     expected.emplace_back({gkernel::Point(8, 0), gkernel::Point(8, 0)});
//     expected.emplace_back({gkernel::Point(9, 6), gkernel::Point(9, 6)});
//     expected.emplace_back({gkernel::Point(9, 5), gkernel::Point(9, 5)});
//     expected.emplace_back({gkernel::Point(9.5, 5.5), gkernel::Point(9.5, 5.5)});
//     expected.emplace_back({gkernel::Point(10, 5), gkernel::Point(10, 5)});
//     expected.emplace_back({gkernel::Point(10, 4), gkernel::Point(10, 4)});
//     expected.emplace_back({gkernel::Point(11, 7), gkernel::Point(11, 7)});
//     expected.emplace_back({gkernel::Point(11, 3), gkernel::Point(11, 3)});
//     expected.emplace_back({gkernel::Point(11, 2), gkernel::Point(11, 2)});
//     expected.emplace_back({gkernel::Point(11.5, 2.5), gkernel::Point(11.5, 2.5)});
//     expected.emplace_back({gkernel::Point(12, 6), gkernel::Point(12, 6)});
//     expected.emplace_back({gkernel::Point(12, 2), gkernel::Point(12, 2)});
//     expected.emplace_back({gkernel::Point(12, 1), gkernel::Point(12, 1)});
//     expected.emplace_back({gkernel::Point(13, 7), gkernel::Point(13, 7)});
//     expected.emplace_back({gkernel::Point(13, 5), gkernel::Point(13, 5)});
//     expected.emplace_back({gkernel::Point(13, 4), gkernel::Point(13, 4)});
//     expected.emplace_back({gkernel::Point(14, 5), gkernel::Point(14, 5)});
//     expected.emplace_back({gkernel::Point(14, 3), gkernel::Point(14, 3)});
//     expected.emplace_back({gkernel::Point(15, 2), gkernel::Point(15, 2)});
//     expected.emplace_back({gkernel::Point(16, 4), gkernel::Point(16, 4)});
//     return expected;
// }

gkernel::SegmentsSet get_segments() {
    gkernel::SegmentsSet segvec;
    segvec.emplace_back({gkernel::Point(3, 12), gkernel::Point(6, 2)});
    segvec.emplace_back({gkernel::Point(3, 12), gkernel::Point(10, 5)});
    segvec.emplace_back({gkernel::Point(6, 2), gkernel::Point(10, 4)});
    segvec.emplace_back({gkernel::Point(6, 12), gkernel::Point(6, 2)});
    segvec.emplace_back({gkernel::Point(6, 12), gkernel::Point(14, 4)});
    segvec.emplace_back({gkernel::Point(10, 11), gkernel::Point(10, 0)});
    segvec.emplace_back({gkernel::Point(10, 11), gkernel::Point(16, 8)});
    segvec.emplace_back({gkernel::Point(10, 5), gkernel::Point(16, 8)});
    segvec.emplace_back({gkernel::Point(10, 0), gkernel::Point(14, 4)});
    segvec.emplace_back({gkernel::Point(14, 12), gkernel::Point(14, 2)});
    segvec.emplace_back({gkernel::Point(14, 12), gkernel::Point(22, 6)});
    segvec.emplace_back({gkernel::Point(16, 8), gkernel::Point(20, 0)});
    segvec.emplace_back({gkernel::Point(14, 2), gkernel::Point(22, 6)});
    segvec.emplace_back({gkernel::Point(22, 6), gkernel::Point(24, 10)});
    segvec.emplace_back({gkernel::Point(22, 6), gkernel::Point(26, 2)});
    segvec.emplace_back({gkernel::Point(20, 0), gkernel::Point(28, 8)});
    segvec.emplace_back({gkernel::Point(24, 10), gkernel::Point(24, 0)});
    segvec.emplace_back({gkernel::Point(24, 10), gkernel::Point(26, 6)});
    segvec.emplace_back({gkernel::Point(24, 0), gkernel::Point(26, 2)});
    segvec.emplace_back({gkernel::Point(26, 2), gkernel::Point(28, 8)});
    return segvec;
}

gkernel::SegmentsSet get_results() {
    gkernel::SegmentsSet expected;
    expected.emplace_back({gkernel::Point(3, 12), gkernel::Point(3, 12)});
    expected.emplace_back({gkernel::Point(6, 2), gkernel::Point(6, 2)});
    expected.emplace_back({gkernel::Point(6, 9), gkernel::Point(6, 9)});
    expected.emplace_back({gkernel::Point(6, 12), gkernel::Point(6, 12)});
    expected.emplace_back({gkernel::Point(10, 11), gkernel::Point(10, 11)});
    expected.emplace_back({gkernel::Point(10, 5), gkernel::Point(10, 5)});
    expected.emplace_back({gkernel::Point(10, 4), gkernel::Point(10, 4)});
    expected.emplace_back({gkernel::Point(10, 0), gkernel::Point(10, 0)});
    expected.emplace_back({gkernel::Point(10, 8), gkernel::Point(10, 8)});
    expected.emplace_back({gkernel::Point(12, 6), gkernel::Point(12, 6)});
    expected.emplace_back({gkernel::Point(14, 12), gkernel::Point(14, 12)});
    expected.emplace_back({gkernel::Point(14, 9), gkernel::Point(14, 9)});
    expected.emplace_back({gkernel::Point(14, 7), gkernel::Point(14, 7)});
    expected.emplace_back({gkernel::Point(14, 4), gkernel::Point(14, 4)});
    expected.emplace_back({gkernel::Point(14, 2), gkernel::Point(14, 2)});
    expected.emplace_back({gkernel::Point(16, 8), gkernel::Point(16, 8)});
    expected.emplace_back({gkernel::Point(18, 4), gkernel::Point(18, 4)});
    expected.emplace_back({gkernel::Point(20, 0), gkernel::Point(20, 0)});
    expected.emplace_back({gkernel::Point(22, 6), gkernel::Point(22, 6)});
    expected.emplace_back({gkernel::Point(24, 10), gkernel::Point(24, 10)});
    expected.emplace_back({gkernel::Point(24, 4), gkernel::Point(24, 4)});
    expected.emplace_back({gkernel::Point(24, 0), gkernel::Point(24, 0)});
    expected.emplace_back({gkernel::Point(26, 2), gkernel::Point(26, 2)});
    expected.emplace_back({gkernel::Point(26, 6), gkernel::Point(26, 6)});
    expected.emplace_back({gkernel::Point(28, 8), gkernel::Point(28, 8)});
    return expected;
}


std::pair<axis_vector, axis_vector> split_coords(const gkernel::SegmentsSet& segments_set) {
    std::vector<gkernel::data_type> x;
    std::vector<gkernel::data_type> y;
    for (std::size_t idx = 0; idx < segments_set.size(); ++idx) {
        x.push_back(segments_set[idx].start().x());
        y.push_back(segments_set[idx].start().y());
        x.push_back(segments_set[idx].end().x());
        y.push_back(segments_set[idx].end().y());
    }
    return std::make_pair(x, y);
}

int main(int argc, char* argv[]) {
    // if (argc != 2) {
    //     throw std::runtime_error("Wrong number of arguments");
    // }

    // auto segments_set = gkernel::FileParser::parseSegmentsSet(argv[1]);

    auto segments_set = get_segments();
    auto intersections = get_results();

    plt::figure_size(resolution_width, resolution_height);
    plt::title("Plot");

    auto split_segments_set = split_coords(segments_set);
    auto split_intersections = split_coords(intersections);

    plt::scatter(split_intersections.first, split_intersections.second, point_marker_size, { { "color", "green" } });
    // plt::scatter(split_segments_set.first, split_segments_set.second, point_marker_size, { { "color", "red" } });

    for (std::size_t idx = 0; idx < split_segments_set.first.size(); idx += 2) {
        plt::plot(std::vector<gkernel::data_type>({split_segments_set.first[idx], split_segments_set.first[idx + 1]}),
                    std::vector<gkernel::data_type>({split_segments_set.second[idx], split_segments_set.second[idx + 1]}));
    }

    plt::show();
    return 0;
}
