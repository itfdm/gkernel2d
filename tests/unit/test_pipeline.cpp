#include "test.hpp"

#include "gkernel/intersection.hpp"
#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"
#include "gkernel/converter.hpp"
#include "gkernel/area_analyzer.hpp"
#include "gkernel/serializer.hpp"

#include <unordered_set>
#include <algorithm>

using namespace gkernel;

void check_result(const SegmentsLayer& actual, const SegmentsLayer& expected) {
    REQUIRE_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < expected.size(); i++) {
        REQUIRE_EQ(actual[i].start(), expected[i].start());
        REQUIRE_EQ(actual[i].end(), expected[i].end());
        REQUIRE_EQ(actual.get_label_value(0, actual[i]), expected.get_label_value(0, expected[i]));
        REQUIRE_EQ(actual.get_label_value(1, actual[i]), expected.get_label_value(1, expected[i]));
        REQUIRE_EQ(actual.get_label_value(2, actual[i]), expected.get_label_value(2, expected[i]));
        REQUIRE_EQ(actual.get_label_value(3, actual[i]), expected.get_label_value(3, expected[i]));
    }
}

void test_areas(const SegmentsLayer& segments_layer) {
    SegmentsSet expected = {{
        {{8, 13}, {16, 13}},
        {{10, 10}, {14, 12}},
        {{14, 12}, {16, 13}},
        {{8, 13}, {10, 10}},
        {{4, 3}, {5, 5}},
        {{5, 5}, {5.5, 6}},
        {{5.5, 6}, {7, 9}},
        {{7, 9}, {9.5, 9}},
        {{9.5, 9}, {12, 9}},
        {{12, 9}, {13.5, 6}},
        {{13.5, 6}, {14, 5}},
        {{14, 5}, {15, 3}},
        {{4, 3}, {10, 3}},
        {{10, 3}, {15, 3}},
        {{8, 6}, {9.5, 9}},
        {{9.5, 9}, {10, 10}},
        {{10, 10}, {11, 12}},
        {{11, 12}, {14, 12}},
        {{14, 12}, {16, 12}},
        {{8, 6}, {12, 9}},
        {{12, 9}, {16, 12}},
        {{2.5, 6}, {5.5, 6}},
        {{5.5, 6}, {8, 6}},
        {{8, 6}, {13.5, 6}},
        {{13.5, 6}, {16, 6}},
        {{10, 3}, {14, 5}},
        {{14, 5}, {16, 6}},
        {{2.5, 6}, {5, 5}},
        {{5, 5}, {10, 3}}
    }};

    expected.set_labels_types({ 0, 1, 2, 3 });

    expected.set_label_values(0, { 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1 });
    expected.set_label_values(1, { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1 });
    expected.set_label_values(2, { 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1 });
    expected.set_label_values(3, { 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 });

    auto layer = Converter::convertToSegmentsLayer(segments_layer);
    auto marked_areas = AreaAnalyzer::findAreas(layer);

    check_result(marked_areas, expected);
}

void test_filter(const SegmentsLayer& segments_layer) {
    SegmentsLayer filtered = AreaAnalyzer::markAreasAndFilter(segments_layer, [](const SegmentsLayer& segments, const Segment& segment) {
        return segments.get_label_value(0, segment) == 1 && segments.get_label_value(1, segment) == 1 &&
                !(segments.get_label_value(2, segment) == 1 && segments.get_label_value(3, segment) == 1) ||
               !(segments.get_label_value(0, segment) == 1 && segments.get_label_value(1, segment) == 1) &&
                segments.get_label_value(2, segment) == 1 && segments.get_label_value(3, segment) == 1;
    });

    SegmentsLayer expected = {{
        {{10, 10}, {14, 12}},
        {{5, 5}, {5.5, 6}},
        {{9.5, 9}, {12, 9}},
        {{13.5, 6}, {14, 5}},
        {{8, 6}, {9.5, 9}},
        {{10, 10}, {11, 12}},
        {{11, 12}, {14, 12}},
        {{8, 6}, {12, 9}},
        {{5.5, 6}, {8, 6}},
        {{8, 6}, {13.5, 6}},
        {{10, 3}, {14, 5}},
        {{5, 5}, {10, 3}}
    }};

    for (size_t i = 0; i < filtered.size(); ++i) {
        REQUIRE_EQ(filtered[i], expected[i]);
    }
}

void TestSimple() {
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
    test_areas(segments_layer);
    test_filter(segments_layer);
}

DECLARE_TEST(TestSimple);
