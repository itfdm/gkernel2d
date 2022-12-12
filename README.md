# Description
gkernel2d is a geometric library containing basic geometry objects, containers, and some algorithms for solving flat geometry problems.\
The goal is to implement algorithms with computational complexity no worse than O(n * log(n)).

# Usage
**CMake options**
```bash
GKERNEL_VISUALIZER - build circuits visualizer
GKERNEL_GENERATOR - build circuits generator
GKERNEL_UNIT_TESTING - enable unit testing
GKERNEL_INTEGRATION_TESTING - enable integration testing
GKERNEL_PERF_TESTING - enable performance testing
GKERNEL_DOCS - build documentation
GKERNEL_STRICT - warnings treated as errors when enabled
```
**How to build project and run tests**
```bash
cmake -DGKERNEL_UNIT_TESTING=ON ..
cmake --build . --config debug
ctest -C debug
```
