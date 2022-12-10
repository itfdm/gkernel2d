# Description
gkernel2d is a geometric library containing basic geometry objects, containers, and some algorithms for solving flat geometry problems.\
The goal is to implement algorithms with computational complexity no worse than O(n * log(n)).

# Usage
**CMake options**
```bash
CCTIC_VISUALIZER - build circuits visualizer
CCTIC_GENERATOR - build circuits generator
CCTIC_UNIT_TESTING - enable unit testing
CCTIC_INTEGRATION_TESTING - enable integration testing
CCTIC_PERF_TESTING - enable performance testing
CCTIC_DOCS - build documentation
CCTIC_STRICT - warnings treated as errors when enabled
```
**How to build project and run tests**
```bash
cmake -DCCTIC_UNIT_TESTING=ON ..
cmake --build . --config debug
ctest -C debug
```
