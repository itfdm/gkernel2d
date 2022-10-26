#include "benchmark/benchmark.h"
#include "gkernel/circuit.h"

// https://github.com/google/benchmark

static void BM_CircuitPush(benchmark::State& state)
{
    for (auto _ : state) {
        gkernel::Circuit circuit(1);
        for (std::size_t idx = 0; idx < 1000; ++idx) {
            circuit.push({ 42, 15 });
        }
    }
}
// Register the function as a benchmark
BENCHMARK(BM_CircuitPush);

BENCHMARK_MAIN();
