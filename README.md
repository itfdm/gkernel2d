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
# Run these commands in the root directory of the repository.
mkdir build
cd build
cmake -DGKERNEL_UNIT_TESTING=ON ..
cmake --build . --config debug
ctest -C debug
```
**Basic usage**

How to create SegmentsSet and set labels
```c++
#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"

using namespace gkernel;

int main() {
    SegmentsSet segments = {{
        {{8, 13}, {16, 13}},
        {{10, 10}, {14, 12}}
    }};
    segments.set_labels_types({ 0 });
    segments.set_label_values(0, { 0, 1});
}
```
How to convert SegmentsSet to SegmentsLayer (find all intersections and split segments)
```c++
#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"
#include "gkernel/converter.hpp"

using namespace gkernel;

int main() {
    gkernel::SegmentsSet input;
    input.emplace_back({gkernel::Point(9, 6), gkernel::Point(10, 5)});
    input.emplace_back({gkernel::Point(6, 1), gkernel::Point(10, 5)});
    input.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(4, 1)});
    input.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(6, 3)});
    
    auto segments_layer = Converter::convertToSegmentsLayer(input);
}
```

How to run full pipeline with labels filtering
```c++
#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"
#include "gkernel/converter.hpp"
#include "gkernel/area_analyzer.hpp"

using namespace gkernel;

int main() {
    Circuit first_circuit = {{
        {{8, 13}, {16, 13}},
        {{16, 13}, {10, 10}},
        {{10, 10}, {8, 13}}
    }};
    Circuit second_circuit = {{
        {{8, 6}, {11, 12}},
        {{11, 12}, {16, 12}},
        {{16, 12}, {8, 6}}
    }};

    Circuit third_circuit = {{
        {{4, 3}, {7, 9}},
        {{7, 9}, {12, 9}},
        {{12, 9}, {15, 3}},
        {{15, 3}, {4, 3}}
    }};

    Circuit fourth_circuit = {{
        {{2.5, 6}, {16, 6}},
        {{16, 6}, {10, 3}},
        {{10, 3}, {2.5, 6}}
    }};

    CircuitsLayer first_layer = {{ first_circuit, third_circuit }};
    CircuitsLayer second_layer = {{ second_circuit, fourth_circuit }};
    auto merged_layers = Converter::mergeCircuitsLayers(first_layer, second_layer);
    auto segments_layer = Converter::convertToSegmentsLayer(merged_layers);
    
    SegmentsLayer filtered = AreaAnalyzer::markAreasAndFilter(segments_layer, [](const SegmentsLayer& segments, const Segment& segment) {
        return segments.get_label_value(0, segment) == 1 && segments.get_label_value(1, segment) == 1 &&
                !(segments.get_label_value(2, segment) == 1 && segments.get_label_value(3, segment) == 1) ||
               !(segments.get_label_value(0, segment) == 1 && segments.get_label_value(1, segment) == 1) &&
                segments.get_label_value(2, segment) == 1 && segments.get_label_value(3, segment) == 1;
    });
}
```
