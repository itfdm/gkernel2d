#include "benchmark/benchmark.h"
#include "objects.hpp"
#include "sweeping_straight.hpp"
#include <vector>
#include <set>
#include <random>

// https://github.com/google/benchmark

std::vector<gkernel::Segment> generateRandomSegments(size_t count)
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_real_distribution<> dis(0, 1000);
    std::vector<gkernel::Segment> segments;
    for (size_t i = 0; i < count; ++i)
    {
        segments.emplace_back(gkernel::Point(dis(gen), dis(gen)), gkernel::Point(dis(gen), dis(gen)));
    }
    return segments;
}

static void BM_segment_set_intersection(benchmark::State &state)
{
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<gkernel::Segment> segments = generateRandomSegments(state.range(0));
        state.ResumeTiming();
        benchmark::DoNotOptimize(gkernel::solve(segments));
    }
}

BENCHMARK(BM_point_intersection)
    ->Unit(benchmark::kMillisecond);

BENCHMARK(BM_segment_set_intersection)
    ->Unit(benchmark::kMicrosecond)
    ->Args({100})
    ->Args({1000})
    ->Args({3000})
    ->Args({5000})
    ->Args({10000});

BENCHMARK_MAIN();