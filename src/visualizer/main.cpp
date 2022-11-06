#define WITHOUT_NUMPY 1

#include <vector>
#include <fstream>

#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using axis_vector = std::vector<gkernel::data_type>;

constexpr double arrow_head_length = 1.;
constexpr double arrow_head_width = 0.2;
constexpr double arrow_zorder = 3.;
constexpr char* facecolor = "red";
constexpr char* edgecolor = "black";

// parse std::vector of gkernel::Point from file in format: x1, y1, x2, y2, ...
std::vector<gkernel::Point> parse_points(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<gkernel::Point> points;
    gkernel::data_type x, y;
    while (file >> x >> y) {
        points.emplace_back(gkernel::Point(x, y));
    }
    return points;
}

std::pair<axis_vector, axis_vector> split_coords(const std::vector<gkernel::Point>& segments) {
    std::vector<gkernel::data_type> x;
    std::vector<gkernel::data_type> y;
    for (std::size_t idx = 0; idx < segments.size(); ++idx) {
        x.push_back(segments[idx].x());
        y.push_back(segments[idx].y());
    }
    return std::make_pair(x, y);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Wrong number of arguments");
    }

    auto segments = parse_points(argv[1]);

    plt::figure_size(1024, 768);
    plt::title("Plot");

    auto split_segments = split_coords(segments);

    for (std::size_t idx = 0; idx < segments.size(); idx += 2) {
        plt::plot(std::vector<double>({split_segments.first[idx], split_segments.first[idx + 1] + 0.01}), std::vector<double>({split_segments.second[idx], split_segments.second[idx + 1] + 0.01}));
    }

    plt::show();
    return 0;
}
