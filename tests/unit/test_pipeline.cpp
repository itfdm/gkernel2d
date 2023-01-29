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

void check_result(const SegmentsSet& actual, const SegmentsSet& expected) {
    REQUIRE_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < expected.size(); i++) {
        REQUIRE_EQ(actual[i].min(), expected[i].min());
        REQUIRE_EQ(actual[i].max(), expected[i].max());
        REQUIRE_EQ(actual.get_label_value(0, actual[i]), expected.get_label_value(0, expected[i]));
        REQUIRE_EQ(actual.get_label_value(1, actual[i]), expected.get_label_value(1, expected[i]));
        REQUIRE_EQ(actual.get_label_value(2, actual[i]), expected.get_label_value(2, expected[i]));
        REQUIRE_EQ(actual.get_label_value(3, actual[i]), expected.get_label_value(3, expected[i]));
    }
}

void check_intersection(const SegmentsSet& actual, const SegmentsSet& expected) {
    REQUIRE_EQ(actual.size(), expected.size());

    for (size_t expected_idx = 0; expected_idx < expected.size(); expected_idx++) {
        bool passed = false;
        for (size_t actual_idx = 0; actual_idx < actual.size(); actual_idx++) {
            if (actual[actual_idx] == expected[expected_idx]) {
                if (actual.get_label_value(0, actual[actual_idx]) == expected.get_label_value(0, expected[expected_idx])) {
                    passed = true;
                    break;
                }
            }
        }
        REQUIRE_EQ(passed, true);
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

    auto marked_areas = AreaAnalyzer::findAreas(segments_layer);
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
    for (std::size_t idx = 0; idx < segments_layer.size(); ++idx) {
        std::cout << segments_layer[idx] << std::endl;
    }
    test_areas(segments_layer);
    test_filter(segments_layer);
}

void test_pipeline_1() {
    Circuit circuit_1 = { {
        {{2, 2}, {2, 10}},
        {{2, 10}, {8, 10}},
        {{8, 10}, {8, 2}},
        {{8, 2}, {2, 2}},
    } };

    Circuit circuit_2 = { {
    {{10, 6}, {12, 9}},
    {{12, 9}, {11, 6}},
    {{11, 6}, {10, 6}}
} };

    Circuit circuit_3 = { {
        {{9, 9}, {12, 12}},
        {{12, 12}, {17, 9}},
        {{17, 9}, {9, 9}}
    } };

    Circuit circuit_4 = { {
        {{9, 1}, {9, 5}},
        {{9, 5}, {15, 5}},
        {{15, 5}, {9, 1}}
    } };

    Circuit circuit_5 = { {
        {{4, 3}, {12, 9}},
        {{12, 9}, {12, 3}},
        {{12, 3}, {4, 3}}
    } };

    Circuit circuit_6 = { {
        {{5, 8}, {5, 12}},
        {{5, 12}, {10, 12}},
        {{10, 12}, {10, 8}},
        {{10, 8}, {5, 8}}
    } };

    Circuit circuit_7 = { {
    {{3, 4}, {3, 8}},
    {{3, 8}, {5, 8}},
    {{5, 8}, {6, 6}},
    {{6, 6}, {3, 4}}
} };

    CircuitsLayer first_layer = { {circuit_1, circuit_2, circuit_3, circuit_4} };
    CircuitsLayer second_layer = { {circuit_5, circuit_6, circuit_7} };
    //====================================================================================================
    //      check segments merging
    //====================================================================================================
    auto merged_layers = Converter::mergeCircuitsLayers(first_layer, second_layer);
    SegmentsSet expected_merged_layers = { {
        {{2, 2}, {2, 10}},
        {{2, 10}, {8, 10}},
        {{8, 10}, {8, 2}},
        {{8, 2}, {2, 2}},

        {{10, 6}, {12, 9}},
        {{12, 9}, {11, 6}},
        {{11, 6}, {10, 6}},

        {{9, 9}, {12, 12}},
        {{12, 12}, {17, 9}},
        {{17, 9}, {9, 9}},

        {{9, 1}, {9, 5}},
        {{9, 5}, {15, 5}},
        {{15, 5}, {9, 1}},

        {{4, 3}, {12, 9}},
        {{12, 9}, {12, 3}},
        {{12, 3}, {4, 3}},

        {{5, 8}, {5, 12}},
        {{5, 12}, {10, 12}},
        {{10, 12}, {10, 8}},
        {{10, 8}, {5, 8}},

        {{3, 4}, {3, 8}},
        {{3, 8}, {5, 8}},
        {{5, 8}, {6, 6}},
        {{6, 6}, {3, 4}},
    } };

    expected_merged_layers.set_labels_types({ 0 });
    for (int i = 0; i < (circuit_1.size() + circuit_2.size() + circuit_3.size() + circuit_4.size()); i++)
        expected_merged_layers.set_label_value(0, expected_merged_layers[i], 0);

    for (int i = (circuit_1.size() + circuit_2.size() + circuit_3.size() + circuit_4.size()); i < expected_merged_layers.size(); i++)
        expected_merged_layers.set_label_value(0, expected_merged_layers[i], 1);

    REQUIRE_EQ(merged_layers.size(), expected_merged_layers.size());
    for (int i = 0; i < merged_layers.size(); i++) {
        REQUIRE_EQ(merged_layers[i].min(), expected_merged_layers[i].min());
        REQUIRE_EQ(merged_layers[i].max(), expected_merged_layers[i].max());
        REQUIRE_EQ(merged_layers.get_label_value(0, merged_layers[i]), expected_merged_layers.get_label_value(0, expected_merged_layers[i]));
    }
    //==========================================================================================================
    //      check conversion to segments layer
    //=========================================================================================================
    SegmentsLayer layer = Converter::convertToSegmentsLayer(merged_layers);
    std::vector<Segment> segments_layer = {
        {{2, 2}, {2, 10}},     
        {{2, 10}, {5, 10}},    
        {{5, 10}, {8, 10}},    
        {{8, 3}, {8, 2}},      
        {{8, 6}, {8, 3}},      
        {{8, 8}, {8, 6}},      
        {{8, 10}, {8, 8}},     
        {{8, 2}, {2, 2}},      

        {{10, 6}, {12, 9}},    
        {{12, 9}, {11, 6}},    
        {{11, 6}, {10, 6}},    

        { {9, 9}, {10, 10}},   
        {{10, 10}, {12, 12}},  
        {{12, 12}, {17, 9}},   
        {{10, 9}, {9, 9}},     
        {{12, 9}, {10, 9}},    
        {{17, 9}, {12, 9}},    

        {{9, 1}, {9, 3}},      
        {{9, 3}, {9, 5}},      
        {{9, 5}, {12, 5}},        
        {{12, 5}, {15, 5}},       
        {{12, 3}, {9, 1}},     
        {{15, 5}, {12, 3}},    
      

        {{4, 3}, {8, 6}},      
        {{8, 6}, {12, 9}},     
        {{12, 5}, {12, 3}},    
        {{12, 9}, {12, 5}},    
        {{8, 3}, {4, 3}},      
        {{9, 3}, {8, 3}},      
        {{12, 3}, {9, 3}},     
 

        {{5, 8}, {5, 10}},     
        {{5, 10}, {5, 12}},    
        {{5, 12}, {10, 12}},
        {{10, 9}, {10, 8}},
        {{10, 10}, {10, 9}},
        {{10, 12}, {10, 10}},
        {{8, 8}, {5, 8}},
        {{10, 8}, {8, 8}},  
 

        {{3, 4}, {3, 8}},
        {{3, 8}, {5, 8}},
        {{5, 8}, {6, 6}},
        {{6, 6}, {3, 4}}    
    };
    SegmentsSet expected_layer(segments_layer);
    expected_layer.set_labels_types({ 0 });

    for (int i = 0; i < 23; i++)
        expected_layer.set_label_value(0, expected_layer[i], 0);

    for (int i = 23; i < expected_layer.size(); i++)
        expected_layer.set_label_value(0, expected_layer[i], 1);

    check_intersection(layer, expected_layer);
    //====================================================================================================
    //      check areas marks
    //====================================================================================================
    auto marked_areas = AreaAnalyzer::findAreas(layer);
    for (int i = 0; i < marked_areas.size(); i++) {
        std::cout << "(" << marked_areas[i].start().x() << ", " << marked_areas[i].start().y() << ") (" << marked_areas[i].end().x() << ", " << marked_areas[i].end().y() << ") " << marked_areas.get_label_value(0, marked_areas[i]) << " " << marked_areas.get_label_value(1, marked_areas[i]) << " " << marked_areas.get_label_value(2, marked_areas[i]) << " " << marked_areas.get_label_value(3, marked_areas[i]) << std::endl;
    }
    SegmentsSet expected_marked_areas(segments_layer);
    expected_marked_areas.set_labels_types({ 0, 1, 2, 3 });
    expected_marked_areas.set_label_values(0, { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1 });
    expected_marked_areas.set_label_values(1, { 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1 });
    expected_marked_areas.set_label_values(2, { 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1 });
    expected_marked_areas.set_label_values(3, { 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0 });
    check_result(marked_areas, expected_marked_areas);
    //====================================================================================================
    //      check filter
    //      areas intersection
    //====================================================================================================
    SegmentsLayer filtered = AreaAnalyzer::markAreasAndFilter(layer, [](const SegmentsSet& seg_set, const Segment& segment) {
        return seg_set.get_label_value(0, segment) == 1 && seg_set.get_label_value(1, segment) == 1 &&
        !(seg_set.get_label_value(2, segment) == 1 && seg_set.get_label_value(3, segment) == 1) ||
        !(seg_set.get_label_value(0, segment) == 1 && seg_set.get_label_value(1, segment) == 1) &&
        seg_set.get_label_value(2, segment) == 1 && seg_set.get_label_value(3, segment) == 1;
        });

    SegmentsSet expected_filtered = { {
        {{5, 10}, {8, 10}},
        {{8, 6}, {8, 3}},
        {{8, 10}, {8, 8}},
        {{10, 6}, {12, 9}},
        {{12, 9}, {11, 6}},
        {{11, 6}, {10, 6}},
        {{9, 9}, {10, 10}},
        {{10, 9}, {9, 9}},
        {{9, 3}, {9, 5}},
        {{9, 5}, {12, 5}},
        {{4, 3}, {8, 6}},
        {{12, 5}, {12, 3}},
        {{8, 3}, {4, 3}},
        {{12, 3}, {9, 3}},
        {{5, 8}, {5, 10}},
        {{10, 10}, {10, 9}},
        {{8, 8}, {5, 8}},
        {{3, 4}, {3, 8}},
        {{3, 8}, {5, 8}},
        {{5, 8}, {6, 6}},
        {{6, 6}, {3, 4}},

        
    } };
    for (size_t i = 0; i < filtered.size(); ++i) {
        REQUIRE_EQ(filtered[i], expected_filtered[i]);
    }
}

DECLARE_TEST(TestSimple);
DECLARE_TEST(test_pipeline_1);


