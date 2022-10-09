#define WITHOUT_NUMPY 1

#include <vector>

#include "gkernel/structures/circuit.h"
#include "file_parser/parser.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using axis_vector = std::vector<gkernel::data_type>;

constexpr double arrow_head_length = 1.;
constexpr double arrow_head_width = 0.2;
constexpr double arrow_zorder = 3.;
constexpr char* facecolor = "red";
constexpr char* edgecolor = "black";

std::pair<axis_vector, axis_vector> split_coords(
    const gkernel::Circuit& circuit) {
    std::vector<gkernel::data_type> x;
    std::vector<gkernel::data_type> y;
    for (std::size_t idx = 0; idx < circuit.size(); ++idx) {
        x.push_back(circuit[idx].x);
        y.push_back(circuit[idx].y);
    }
    return std::make_pair(x, y);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Wrong number of arguments");
    }

    auto circuits = FileParser::parse(argv[1]);

    plt::figure_size(gkernel::resolution_width, gkernel::resolution_height);
    plt::title("Plot");

    for (auto& circuit : circuits) {
        auto split_circuit = split_coords(circuit);

        double x_begin = static_cast<double>(split_circuit.first[0]);
        double y_begin = static_cast<double>(split_circuit.second[0]);
        double dx = static_cast<double>(split_circuit.first[1]) - x_begin;
        double dy = static_cast<double>(split_circuit.second[1]) - y_begin;

        plt::arrow(x_begin, y_begin, dx / 2, dy / 2, facecolor, edgecolor,
                   arrow_head_length, arrow_head_width, arrow_zorder);
        plt::plot(split_circuit.first, split_circuit.second);
        plt::text(x_begin, y_begin, std::to_string(circuit.getID()));
    }

    plt::show();
    return 0;
}
