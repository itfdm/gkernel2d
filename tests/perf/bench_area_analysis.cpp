#include "benchmark/benchmark.h"
#include "gkernel/objects.hpp"
#include "gkernel/intersection.hpp"
#include "gkernel/containers.hpp"
#include "gkernel/helpers.hpp"
#include "gkernel/area_analyzer.hpp"
#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <set>
#include <chrono>

gkernel::SegmentsLayer generate_layers(int circuit_count)
{
    gkernel::data_type x = 0;
    std::vector<gkernel::Segment> segments;
    segments.emplace_back(gkernel::Point(0, 2), gkernel::Point(1.5, 3.5));
    segments.emplace_back(gkernel::Point(1.5, 3.5), gkernel::Point(2, 4));
    segments.emplace_back(gkernel::Point(2, 4), gkernel::Point(2.5, 3.5));
    segments.emplace_back(gkernel::Point(2.5, 3.5), gkernel::Point(3, 3));
    segments.emplace_back(gkernel::Point(3, 3), gkernel::Point(4, 2));
    segments.emplace_back(gkernel::Point(4, 2), gkernel::Point(3, 1));
    segments.emplace_back(gkernel::Point(3, 1), gkernel::Point(2, 0));
    segments.emplace_back(gkernel::Point(2, 0), gkernel::Point(0, 2));

    segments.emplace_back(gkernel::Point(0, 5), gkernel::Point(2, 7));
    segments.emplace_back(gkernel::Point(2, 7), gkernel::Point(3, 6));
    segments.emplace_back(gkernel::Point(3, 6), gkernel::Point(4, 5));
    segments.emplace_back(gkernel::Point(4, 5), gkernel::Point(3, 4));
    segments.emplace_back(gkernel::Point(3, 4), gkernel::Point(2.5, 3.5));
    segments.emplace_back(gkernel::Point(2.5, 3.5), gkernel::Point(2, 3));
    segments.emplace_back(gkernel::Point(2, 3), gkernel::Point(1.5, 3.5));
    segments.emplace_back(gkernel::Point(1.5, 3.5), gkernel::Point(0, 5));

    x += 2;
    int seg_count = circuit_count;
    for (int i = 0; i < seg_count; i++)
    {
        segments.emplace_back(gkernel::Point(x, 2), gkernel::Point(x + 1, 3));
        segments.emplace_back(gkernel::Point(x + 1, 3), gkernel::Point(x + 1.5, 3.5));
        segments.emplace_back(gkernel::Point(x + 1.5, 3.5), gkernel::Point(x + 2, 4));
        segments.emplace_back(gkernel::Point(x + 2, 4), gkernel::Point(x + 2.5, 3.5));
        segments.emplace_back(gkernel::Point(x + 2.5, 3.5), gkernel::Point(x + 3, 3));
        segments.emplace_back(gkernel::Point(x + 3, 3), gkernel::Point(x + 4, 2));
        segments.emplace_back(gkernel::Point(x + 4, 2), gkernel::Point(x + 3, 1));
        segments.emplace_back(gkernel::Point(x + 3, 1), gkernel::Point(x + 2, 0));
        segments.emplace_back(gkernel::Point(x + 2, 0), gkernel::Point(x + 1, 1));
        segments.emplace_back(gkernel::Point(x + 1, 1), gkernel::Point(x, 2));

        segments.emplace_back(gkernel::Point(x, 5), gkernel::Point(x + 1, 6));
        segments.emplace_back(gkernel::Point(x + 1, 6), gkernel::Point(x + 2, 7));
        segments.emplace_back(gkernel::Point(x + 2, 7), gkernel::Point(x + 3, 6));
        segments.emplace_back(gkernel::Point(x + 3, 6), gkernel::Point(x + 4, 5));
        segments.emplace_back(gkernel::Point(x + 4, 5), gkernel::Point(x + 3, 4));
        segments.emplace_back(gkernel::Point(x + 3, 4), gkernel::Point(x + 2.5, 3.5));
        segments.emplace_back(gkernel::Point(x + 2.5, 3.5), gkernel::Point(x + 2, 3));
        segments.emplace_back(gkernel::Point(x + 2, 3), gkernel::Point(x + 1.5, 3.5));
        segments.emplace_back(gkernel::Point(x + 1.5, 3.5), gkernel::Point(x + 1, 4));
        segments.emplace_back(gkernel::Point(x + 1, 4), gkernel::Point(x, 5));
        x += 2;
    }
    gkernel::label_data_type layer_num = 0;

    gkernel::SegmentsSet input = segments;
    input.set_labels_types({0});

    for (int i = 0; i < 8; i++)
        input.set_label_value(0, input[i], layer_num);

    layer_num = 1;

    for (int i = 8; i < 16; i++)
        input.set_label_value(0, input[i], layer_num);

    for (std::size_t idx = 16; idx < input.size(); idx += 20)
    {
        for (std::size_t j = 0; j < 10; j++)
            input.set_label_value(0, input[idx + j], layer_num);
        layer_num = 0 ? 1 : 0;
        for (std::size_t j = 10; j < 20; j++)
            input.set_label_value(0, input[idx + j], layer_num);
    }

    gkernel::SegmentsLayer layer = gkernel::AreaAnalysis::findSegmentsNeighbours(input);
    return layer;
}

static void BM_mark_area(benchmark::State &state)
{
    gkernel::SegmentsLayer test_layer = generate_layers(state.range(0));

    for (auto _ : state)
    {
        gkernel::SegmentsLayer result = gkernel::AreaAnalysis::parallelMarkAreas(test_layer);
    }
}

BENCHMARK(BM_mark_area)
    ->Unit(benchmark::kMillisecond)
    ->Args({5000})
    ->Args({7000})
    ->Args({10000})
    ->Args({20000})
    ->Args({50000})
    ->Args({100000})
    ->Iterations(1);

BENCHMARK_MAIN();