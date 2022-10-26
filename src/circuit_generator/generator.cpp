#include <algorithm>
#include <set>
#include <cmath>

#include "gkernel/circuit.h"
#include "output_serializer/serializer.h"

bool circuitIdentification(const gkernel::Circuit& circuit) {
    long long result = 0;
    size_t number_of_points = circuit.size();

    for (size_t i = 0; i < number_of_points - 1; ++i) {
        result += (static_cast<long long>(circuit[i + 1].x) - static_cast<long long>(circuit[i].x)) *
                  (static_cast<long long>(circuit[i].y) + static_cast<long long>(circuit[i + 1].y));
    }
    return result > 0;
}

enum class GenerateMode { RANDOM, NONINTERSECTING };

enum class TraversalMode { RANDOM, FORWARD, BACKWARD };

struct BoundingBox {
    bool empty() {
        return width == 0 && height == 0;
    }

    gkernel::Point left_bottom_point;
    size_t width;
    size_t height;
};

struct GenParameters {
    GenerateMode gen_mode{};
    TraversalMode traversal{};
    size_t circuits_num = 1;
    size_t min_circuit_dim = 3;
    size_t max_circuit_dim = 15;
    std::string output_path = "output.txt";
};

struct xorshift128_state {
    uint32_t a, b, c, d;
};

uint32_t xorshift128() {
    /* Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs" */
    static xorshift128_state state{1, 2, 3, 4};

    uint32_t t = state.d;
    uint32_t const s = state.a;
    state.d = state.c;
    state.c = state.b;
    state.b = s;

    t ^= t << 11;
    t ^= t >> 8;
    return state.a = t ^ s ^ (s >> 19);
}

bool getPosPointRelativeLine(gkernel::Point point,
                             gkernel::Point begin_line,
                             gkernel::Point end_line) {
    return (static_cast<long long>(end_line.x - begin_line.x) * static_cast<long long>(point.y - begin_line.y) -
            static_cast<long long>(end_line.y - begin_line.y) * static_cast<long long>(point.x - begin_line.x)) > 0;
}

std::vector<gkernel::Point> generatePoints(size_t points_num,
                                         const BoundingBox& bounding_box) {
    std::vector<gkernel::Point> points;
    points.reserve(points_num);
    bool valid_points = false;
    while (!valid_points) {
        valid_points = true;
        for (size_t i = 0; i < points_num; ++i) {
            points.emplace_back(gkernel::Point{
                bounding_box.left_bottom_point.x +
                    static_cast<gkernel::data_type>(xorshift128() %
                                                  bounding_box.width),
                bounding_box.left_bottom_point.y +
                    static_cast<gkernel::data_type>(xorshift128() %
                                                  bounding_box.height)});
        }
        valid_points &= !std::all_of(points.begin(), points.end(), [&](const gkernel::Point &p) {
            return p.x == points.front().x;
        });
        valid_points &= !std::all_of(points.begin(), points.end(), [&](const gkernel::Point &p) {
            return p.y == points.front().y;
        });
    }
    return points;
}

template <template <class> class Comparator = std::less>
struct PointComparator {
    PointComparator() {}

    bool operator()(const gkernel::Point& p1, const gkernel::Point& p2) const {
        return p1.x == p2.x ? Comparator<gkernel::data_type>{}(p1.y, p2.y)
                            : Comparator<gkernel::data_type>{}(p1.x, p2.x);
    }
};

gkernel::Circuit createCircuit(gkernel::identifier id, size_t circuit_dim,
                             const BoundingBox& bounding_box) {
    std::vector<gkernel::Point> points = generatePoints(circuit_dim, bounding_box);

    gkernel::Point leftmost_point =
        *std::min_element(points.begin(), points.end(), PointComparator{});
    gkernel::Point rightmost_point =
        *std::max_element(points.begin(), points.end(), PointComparator{});

    std::vector<gkernel::Point> first_part, second_part;
    first_part.reserve(circuit_dim);
    second_part.reserve(circuit_dim);
    for (const auto& point : points) {
        if (point == leftmost_point || point == rightmost_point) {
            continue;
        }
        if (getPosPointRelativeLine(point, leftmost_point, rightmost_point)) {
            first_part.emplace_back(point);
        } else {
            second_part.emplace_back(point);
        }
    }
    std::sort(first_part.begin(), first_part.end(), PointComparator{});
    std::sort(second_part.begin(), second_part.end(), PointComparator<std::greater>{});

    gkernel::Circuit circuit(id);
    circuit.push(leftmost_point);
    circuit.push(first_part);
    circuit.push(rightmost_point);
    circuit.push(second_part);
    circuit.push(leftmost_point);

    return circuit;
}

std::pair<size_t, size_t> getSeparatorsNum(size_t circuits_num) {
    size_t first_factor{circuits_num}, second_factor{1};
    for (size_t i = 2; i <= std::sqrt(circuits_num); ++i) {
        if (circuits_num % i == 0) {
            first_factor = circuits_num / i;
            second_factor = i;
        }
    }
    return {first_factor, second_factor};
}

bool validateArea(size_t weight, size_t height, size_t circuit_dim) {
    return (weight > static_cast<size_t>(circuit_dim / std::log2(gkernel::resolution_width))) &&
           (height > static_cast<size_t>(circuit_dim / std::log2(gkernel::resolution_height))) &&
           weight > 2 && height > 2;
}

BoundingBox getSatisfyInternalBoundingBox(gkernel::Circuit& circuit, size_t new_circuit_dim) {
    size_t rightmost_point_idx = std::distance(
        circuit.begin(),
        std::find(circuit.begin(), circuit.end(), circuit.getRightmostPoint()));

    if (rightmost_point_idx < 3 || rightmost_point_idx > circuit.size() - 4) {
        return BoundingBox{};
    }

    bool satisfy_req_area = false;
    size_t edit_counter = 0;
    while (!satisfy_req_area) {
        auto high_lowest_point_idx = std::distance(
            circuit.begin(),
            std::min_element(
                circuit.begin() + 1, circuit.begin() + rightmost_point_idx,
                [](const gkernel::Point& p1, const gkernel::Point& p2) {
                    return p1.y < p2.y;
                }));

        auto low_highest_point_idx = std::distance(
            circuit.begin(),
            std::max_element(
                circuit.begin() + rightmost_point_idx + 1, circuit.end() - 1,
                [](const gkernel::Point& p1, const gkernel::Point& p2) {
                    return p1.y < p2.y;
                }));

        if (circuit[high_lowest_point_idx].y < circuit[low_highest_point_idx].y) {
            return BoundingBox{};
        }
        size_t curr_height = circuit[high_lowest_point_idx].y - circuit[low_highest_point_idx].y;

        size_t right_border = std::min(circuit[rightmost_point_idx - 1].x,
                                       circuit[rightmost_point_idx + 1].x);
        size_t left_border = std::max(circuit[1].x, circuit[circuit.size() - 2].x);
        size_t curr_weight = right_border > left_border ? right_border - left_border
                                                        : left_border - right_border;

        satisfy_req_area = validateArea(curr_weight, curr_height, new_circuit_dim);

        if (satisfy_req_area) {
            return BoundingBox{{static_cast<gkernel::data_type>(std::min(left_border, right_border) + 1),
                                circuit[low_highest_point_idx].y + 1},
                                curr_weight - 1,
                                curr_height - 1};
        } else {
            gkernel::Point updated_low_point{circuit[low_highest_point_idx].x,
                                           circuit.getLowestPoint().y};
            circuit.editPoint(low_highest_point_idx, updated_low_point);

            gkernel::Point updated_high_point{circuit[high_lowest_point_idx].x,
                                            circuit.getHighestPoint().y};
            circuit.editPoint(high_lowest_point_idx, updated_high_point);
        }

        ++edit_counter;
        if (edit_counter == circuit.size()) {
            return BoundingBox{};
        }
    }
}

std::vector<gkernel::Circuit> generateCircuits(GenParameters params) {
    BoundingBox plane{{0, 0}, gkernel::resolution_width, gkernel::resolution_height};
    plane.width *= static_cast<size_t>(std::pow(std::log(params.max_circuit_dim), 3));
    plane.height *= static_cast<size_t>(std::pow(std::log(params.max_circuit_dim), 3));

    std::vector<gkernel::Circuit> circuits;
    circuits.reserve(params.circuits_num);

    if (params.gen_mode == GenerateMode::RANDOM) {
        for (size_t i = 0; i < params.circuits_num; ++i) {
            size_t circuit_dim = xorshift128() %
                                 (params.max_circuit_dim - params.min_circuit_dim + 1) +
                                 params.min_circuit_dim;
            size_t rand_x{}, rand_y{}, rand_width{}, rand_height{};

            bool satisfy_req_area = false;
            while (!satisfy_req_area) {
                rand_x = xorshift128() % plane.width;
                rand_y = xorshift128() % plane.height;
                rand_width = xorshift128() % (plane.width - rand_x);
                rand_height = xorshift128() % (plane.height - rand_y);
                satisfy_req_area = validateArea(rand_width, rand_height, params.max_circuit_dim);
            }
            BoundingBox rand_bb{{static_cast<gkernel::data_type>(rand_x),
                                 static_cast<gkernel::data_type>(rand_y)},
                                rand_width,
                                rand_height};
            circuits.emplace_back(
                createCircuit(i, circuit_dim, rand_bb));
        }
    } else {  // GenerateMode::NONINTERSECTING case
        std::pair<size_t, size_t> separators = getSeparatorsNum(params.circuits_num);
        size_t stride_x = plane.width / separators.first;
        size_t stride_y = plane.height / separators.second;

        auto create_default_bounding_box = [&](size_t id) -> BoundingBox {
            size_t offset_x = id % separators.first * stride_x;
            size_t offset_y = id / separators.first * stride_y;
            return {{static_cast<gkernel::data_type>(offset_x),
                     static_cast<gkernel::data_type>(offset_y)},
                     stride_x,
                     stride_y};
        };
        std::set<size_t> used_circuits_indices;

        for (size_t i = 0; i < params.circuits_num; ++i) {
            size_t circuit_dim = xorshift128() %
                                 (params.max_circuit_dim - params.min_circuit_dim + 1) +
                                 params.min_circuit_dim;
            if (xorshift128() % 2 == 0 && !circuits.empty()) {
                size_t suitable_circuit_idx{};
                BoundingBox bounding_box{};
                size_t attempt_counter = 0;
                while (bounding_box.empty()) {
                    ++attempt_counter;
                    suitable_circuit_idx = xorshift128() % circuits.size();
                    if (used_circuits_indices.count(suitable_circuit_idx) == 0) {
                        bounding_box = getSatisfyInternalBoundingBox(circuits[suitable_circuit_idx], circuit_dim);
                    }
                    if (attempt_counter > circuits.size()) {
                        bounding_box = create_default_bounding_box(i);
                    }
                }
                used_circuits_indices.emplace(suitable_circuit_idx);
                circuits.emplace_back(
                    createCircuit(i, circuit_dim, bounding_box));
            } else {
                circuits.emplace_back(
                    createCircuit(i, circuit_dim, create_default_bounding_box(i)));
            }
        }
    }

    switch (params.traversal) {
        case TraversalMode::FORWARD: {
            for (auto& circuit : circuits) {
                if (!circuitIdentification(circuit)) {
                    circuit.reverse();
                }
            }
            break;
        }
        case TraversalMode::BACKWARD: {
            for (auto& circuit : circuits) {
                if (circuitIdentification(circuit)) {
                    circuit.reverse();
                }
            }
            break;
        }
        case TraversalMode::RANDOM: {
            break;
        }
    }
    return circuits;
}

int main(int argc, char* argv[]) {
    try {
        if (argc == 1) {
            // clang-format off
            std::cout << "Usage: <1> <2> <3> <4> <5> <6>\n"
                         "1 - generator mode: 0 - RANDOM, 1 - NONINTERSECTING\n"
                         "2 - traversal: 0 - RANDOM, 1 - FORWARD, 2 - BACKWARD\n"
                         "3 - number of circuits\n"
                         "4 - minimal circuit dimension\n"
                         "5 - maximal circuit dimension\n"
                         "6 - path to output file" << std::endl;
            exit(1);
            // clang-format on
        }
        if (argc == 5) {
            throw std::runtime_error("Wrong number of arguments");
        }
        GenParameters params;
        switch (argc) {
            case 7:
                params.output_path = argv[6];
            case 6:
                params.max_circuit_dim = std::stoi(argv[5]);
                params.min_circuit_dim = std::stoi(argv[4]);
            case 4:
                params.circuits_num = std::stoi(argv[3]);
            case 3:
                params.traversal = static_cast<TraversalMode>(std::stoi(argv[2]));
            case 2:
                params.gen_mode = static_cast<GenerateMode>(std::stoi(argv[1]));
        }
        std::vector<gkernel::Circuit> generated_circuits = generateCircuits(params);
        OutputSerializer::serializeCircuits(generated_circuits, params.output_path);
    } catch (std::runtime_error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        exit(-1);
    }

    return 0;
}
