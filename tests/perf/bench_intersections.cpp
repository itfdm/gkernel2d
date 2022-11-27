#include "benchmark/benchmark.h"
#include "gkernel/objects.hpp"
#include "gkernel/intersection.hpp"
#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <set>

// https://github.com/google/benchmark

struct xorshift128_state
{
    uint32_t a, b, c, d;
};

uint32_t xorshift128()
{
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

std::vector<gkernel::Segment> generateRandomSegments(size_t count, int w_width, int w_height)
{
    int window_width = w_width;
    int window_height = w_height;

    std::vector<gkernel::Segment> segments;
    for (size_t i = 0; i < count; ++i)
    {
        segments.emplace_back(gkernel::Point(xorshift128() % window_width, xorshift128() % window_height),
                              gkernel::Point(xorshift128() % window_width, xorshift128() % window_height));
    }
    return segments;
}

static void BM_segment_set_intersection(benchmark::State &state)
{
    std::vector<gkernel::Segment> result;
    result.reserve(1000000);
    int window_width = 1000;
    int window_height = 50;

    double long_side = std::max(window_width, window_height);
    double mean_rel_length = 0;
    double rel_length = 0;
    size_t bet_0_25 = 0, bet_25_50 = 0, bet_50_80 = 0, bet_80_100 = 0;
    std::vector<double> rel_length_vec;

    std::vector<gkernel::Segment> segments = generateRandomSegments(state.range(0), window_width, window_height);
    for (auto _ : state)
    {
        state.PauseTiming();
        result.clear();
        state.ResumeTiming();
        gkernel::intersectSetSegmentsBruteForce(segments, [&result](const gkernel::Segment& first, const gkernel::Segment& second, const gkernel::Segment& intersection) {
            result.push_back(intersection);
            return true;
        });
        std::sort(result.begin(), result.end(), [](const gkernel::Segment& a, const gkernel::Segment& b) {
            if (a.start().x() != b.start().x()) {
                return a.start().x() < b.start().x();
            } else {
                return a.start().y() < b.start().y();
            }
        });
        auto last = std::unique(result.begin(), result.end(), [](const gkernel::Segment& a, const gkernel::Segment& b) {
            bool result = std::abs(a.start().x() - b.start().x()) < 1e-6 && std::abs(a.start().y() - b.start().y()) < 1e-6;
            return result;
        });
        result.erase(last, result.end());
        // std::cout << "result size: " << result.size() << std::endl;
        // benchmark::DoNotOptimize(result = std::move(gkernel::solve(segments)));
    }

    // average relative segments length
    for (auto seg : segments)
    {
        rel_length = std::sqrt(std::pow(seg.start().x() - seg.end().x(), 2) + std::pow(seg.start().y() - seg.end().y(), 2)) / (double)long_side;
        if (rel_length > 1)
            rel_length = 1;
        rel_length_vec.push_back(rel_length);
    }
    double sum = 0;
    mean_rel_length = std::accumulate(rel_length_vec.begin(), rel_length_vec.end(), sum) / (double)segments.size();
    for (double x : rel_length_vec)
    {
        if (x <= 0.25)
        {
            bet_0_25++;
            continue;
        }
        if (x <= 0.5)
        {
            bet_25_50++;
            continue;
        }
        if (x <= 0.8)
        {
            bet_50_80++;
            continue;
        }
        if (x <= 1)
        {
            bet_80_100++;
            continue;
        }
    }

    state.counters["intersections"] = result.size();
    state.counters["mean_rel_length"] = mean_rel_length;
    state.counters["length 0-25%"] = bet_0_25;
    state.counters["length 25-50%"] = bet_25_50;
    state.counters["length 50-80%"] = bet_50_80;
    state.counters["length 80-100%"] = bet_80_100;
}
BENCHMARK(BM_segment_set_intersection)
    ->Unit(benchmark::kMillisecond)
    ->Args({100})
    ->Args({1000})
    ->Args({3000})
    ->Args({5000})
    ->Args({10000})
    ->Args({100000})
    ->Args({250000})
    ->Args({500000})
    ->Args({750000})
    ->Args({1000000})
    ->Args({2000000})
    ->Args({10000000});

BENCHMARK_MAIN();
