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

gkernel::SegmentsLayer generate_layers()
{
    gkernel::data_type x = 0;
    std::vector<gkernel::Segment> segments;
    segments.emplace_back(gkernel::Point(x, 2), gkernel::Point(2, 4));
    segments.emplace_back(gkernel::Point(x + 2, 0), gkernel::Point(x, 2));
    segments.emplace_back(gkernel::Point(x + 2, 2), gkernel::Point(x + 3, 3));
    segments.emplace_back(gkernel::Point(x + 3, 3), gkernel::Point(x + 4, 2));
    segments.emplace_back(gkernel::Point(x + 4, 2), gkernel::Point(x + 3, 1));
    segments.emplace_back(gkernel::Point(x + 3, 1), gkernel::Point(x + 2, 0));
    x += 2;

    int seg_count = 5000;
    for (int i = 0; i < seg_count; i++)
    {
        segments.emplace_back(gkernel::Point(x, 2), gkernel::Point(x + 1, 3));
        segments.emplace_back(gkernel::Point(x + 1, 3), gkernel::Point(x + 2, 4));
        segments.emplace_back(gkernel::Point(x + 2, 4), gkernel::Point(x + 3, 3));
        segments.emplace_back(gkernel::Point(x + 3, 3), gkernel::Point(x + 4, 2));
        segments.emplace_back(gkernel::Point(x + 4, 2), gkernel::Point(x + 3, 1));
        segments.emplace_back(gkernel::Point(x + 3, 1), gkernel::Point(x + 2, 0));
        segments.emplace_back(gkernel::Point(x + 2, 0), gkernel::Point(x + 1, 1));
        segments.emplace_back(gkernel::Point(x + 1, 1), gkernel::Point(x, 2));
        x += 2;
    }

    gkernel::label_data_type layer_num = 0;

    gkernel::SegmentsSet input = segments;
    input.set_labels_types({0});

    for (int i = 0; i < 6; i++)
        input.set_label_value(0, input[i], layer_num);

    layer_num = 1;

    for (std::size_t idx = 6; idx < input.size(); idx += 8)
    {
        for (std::size_t j = 0; j < 8; j++)
            input.set_label_value(0, input[idx + j], layer_num);
        layer_num = 0 ? 1 : 0;
    }

    gkernel::SegmentsLayer layer = gkernel::AreaAnalysis::findSegmentsNeighbours(input);
    return layer;
}

static void BM_mark_area(benchmark::State &state)
{
    gkernel::SegmentsLayer test_layer = generate_layers();

    for (auto _ : state)
    {
        // gkernel::SegmentsLayer result = gkernel::AreaAnalysis::parallelMarkAreas(test_layer);
        gkernel::SegmentsLayer result1 = gkernel::AreaAnalysis::markAreas(test_layer);
    }
}

BENCHMARK(BM_mark_area)
    ->Unit(benchmark::kMillisecond)
    ->Iterations(1);

BENCHMARK_MAIN();

//  if (result.size() != result1.size())
//      std::cout << " Different sizes" << std::endl;

// for (std::size_t idx = 0; idx < result.size(); ++idx)
// {
//     if (result.get_label_value(0, result[idx]) != result1.get_label_value(0, result[idx]))
//     {
//         std::cout << "Error" << std::endl;
//         break;
//     }
//     if (result.get_label_value(1, result[idx]) != result1.get_label_value(1, result[idx]))
//     {
//         std::cout << "Error" << std::endl;
//         break;
//     }
//     if (result.get_label_value(2, result[idx]) != result1.get_label_value(2, result[idx]))
//     {
//         std::cout << "Error" << std::endl;
//         break;
//     }
//     if (result.get_label_value(3, result[idx]) != result1.get_label_value(3, result[idx]))
//     {
//         std::cout << "Error" << std::endl;
//         break;
//     }
// }