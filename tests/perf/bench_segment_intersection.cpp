#include "benchmark/benchmark.h"
#include "gkernel/objects.hpp"
#include "gkernel/intersection.hpp"
#include <vector>
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

std::vector<gkernel::Segment> generateRandomSegments(size_t count)
{
    int window_width = 1000;
    int window_height = 100;

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
    for (auto _ : state)
    {
        state.PauseTiming();
        std::vector<gkernel::Segment> segments = generateRandomSegments(state.range(0));
        state.ResumeTiming();
        benchmark::DoNotOptimize(gkernel::solve(segments));
    }
}
BENCHMARK(BM_segment_set_intersection)
    ->Unit(benchmark::kMillisecond)
    ->Args({100})
    ->Args({1000})
    ->Args({3000})
    ->Args({5000})
    ->Args({10000});

BENCHMARK_MAIN();