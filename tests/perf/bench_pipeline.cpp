#include "benchmark/benchmark.h"
#include "gkernel/objects.hpp"
#include "gkernel/intersection.hpp"
#include "gkernel/converter.hpp"
#include "gkernel/area_analyzer.hpp"
#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <set>

struct xorshift128_state {
    uint32_t a, b, c, d;
};

class xorshift128 {
    xorshift128_state state{1, 2, 3, 4};
public:
    uint32_t random() {
        uint32_t t = state.d;
        uint32_t const s = state.a;
        state.d = state.c;
        state.c = state.b;
        state.b = s;

        t ^= t << 11;
        t ^= t >> 8;
        return state.a = t ^ s ^ (s >> 19);
    }
};

xorshift128 random;

// generate segments with length
gkernel::SegmentsSet generateRandomSegments(size_t count, int w_width, int w_height, int length) {
    int window_width = w_width;
    int window_height = w_height;

    gkernel::SegmentsSet segments;
    for (size_t idx = 0; idx < count; ++idx)
    {
        int x1 = random.random() % window_width;
        int y1 = random.random() % window_height;
        int x2 = x1 + random.random() % length + 1;
        int y2 = y1 + random.random() % length + 1;
        segments.emplace_back({gkernel::Point(x1, y1), gkernel::Point(x2, y2)});
    }
    return segments;
}

class SegmentsComparator {
public:
    bool operator()(const gkernel::Segment& lhs, const gkernel::Segment& rhs) const {
        if (lhs.start() != rhs.start()) {
            if (lhs.start().x() != rhs.start().x()) {
                return lhs.start().x() < rhs.start().x();
            } else {
                return lhs.start().y() < rhs.start().y();
            }
        } else {
            if (lhs.end().x() != rhs.end().x()) {
                return lhs.end().x() < rhs.end().x();
            } else {
                return lhs.end().y() < rhs.end().y();
            }
        }
        return false;
    }
};

static void BM_full_pipeline(benchmark::State &state) {
    int window_width = 10000;
    int window_height = 10000;

    gkernel::SegmentsSet first_set = generateRandomSegments(state.range(0) / 2, window_width, window_height, 25);
    gkernel::SegmentsSet second_set = generateRandomSegments(state.range(0) / 2, window_width, window_height, 25);

    gkernel::SegmentsLayer first_layer = gkernel::Converter::convertToSegmentsLayer(first_set);
    gkernel::SegmentsLayer second_layer = gkernel::Converter::convertToSegmentsLayer(second_set);

    std::vector<gkernel::Segment> first_circuit_vector;
    first_circuit_vector.reserve(first_layer.size());

    for (std::size_t idx = 0; idx < first_layer.size(); ++idx) {
        first_circuit_vector.push_back(first_layer[idx]);
    }

    std::vector<gkernel::Segment> second_circuit_vector;
    second_circuit_vector.reserve(second_layer.size());

    for (std::size_t idx = 0; idx < second_layer.size(); ++idx) {
        second_circuit_vector.push_back(second_layer[idx]);
    }

    gkernel::Circuit first_circuit(first_circuit_vector, false);
    gkernel::Circuit second_circuit(second_circuit_vector, false);

    gkernel::CircuitsLayer first_circuits_layer = {{ first_circuit }};
    gkernel::CircuitsLayer second_circuits_layer = {{ second_circuit }};

    gkernel::SegmentsSet merged_circuits = gkernel::Converter::mergeCircuitsLayers(first_circuits_layer, second_circuits_layer);

    for (auto _ : state) {
        gkernel::SegmentsLayer segments_layer = gkernel::Converter::convertToSegmentsLayer(merged_circuits);
        gkernel::SegmentsLayer filtered = gkernel::AreaAnalyzer::filterSegmentsByLabels(segments_layer, [](const gkernel::SegmentsLayer& segments, const gkernel::Segment& segment) {
            return segments.get_label_value(0, segment) == 1 && segments.get_label_value(1, segment) == 1 &&
                    !(segments.get_label_value(2, segment) == 1 && segments.get_label_value(3, segment) == 1) ||
                !(segments.get_label_value(0, segment) == 1 && segments.get_label_value(1, segment) == 1) &&
                    segments.get_label_value(2, segment) == 1 && segments.get_label_value(3, segment) == 1;
        });
        state.counters["input_size"] = static_cast<double>(merged_circuits.size());
        state.counters["result_size"] = static_cast<double>(filtered.size());
    }
}

BENCHMARK(BM_full_pipeline)
->Unit(benchmark::kMillisecond)
    ->Args({100})
    ->Args({1000})
    ->Args({3000})
    ->Args({5000})
    ->Args({10000})
    ->Args({20000})
    ->Args({100000})
    ->Args({250000});

BENCHMARK_MAIN();
