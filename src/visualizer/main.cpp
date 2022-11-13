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

std::pair<axis_vector, axis_vector> split_coords(const gkernel::SegmentsSet& segments_set) {
    std::vector<gkernel::data_type> x;
    std::vector<gkernel::data_type> y;
    for (std::size_t idx = 0; idx < segments_set.size(); ++idx) {
        x.push_back(segments_set[idx].begin_point().x());
        y.push_back(segments_set[idx].begin_point().y());
        x.push_back(segments_set[idx].end_point().x());
        y.push_back(segments_set[idx].end_point().y());
    }
    return std::make_pair(x, y);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Wrong number of arguments");
    }

    auto segments_set = gkernel::FileParser::parseSegmentsSet(argv[1]);

    plt::figure_size(resolution_width, resolution_height);
    plt::title("Plot");

    auto split_segments_set = split_coords(segments_set);

    plt::scatter(split_segments_set.first, split_segments_set.second, point_marker_size, { { "color", "red" } });

    for (std::size_t idx = 0; idx < split_segments_set.first.size(); idx += 2) {
        plt::plot(std::vector<gkernel::data_type>({split_segments_set.first[idx], split_segments_set.first[idx + 1]}),
                    std::vector<gkernel::data_type>({split_segments_set.second[idx], split_segments_set.second[idx + 1]}));
    }

    plt::show();
    return 0;
}
