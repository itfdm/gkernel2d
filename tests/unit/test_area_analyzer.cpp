#include "test.hpp"
#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"
#include "gkernel/area_analyzer.hpp"
#include "gkernel/serializer.hpp"

using namespace gkernel;

enum test_labels {
    circuits_layer_id = 0
};

void TestAreasSimple() {
    std::vector<Segment> segments = {
        {{0, 0}, {1, 1}},
        {{0, 1}, {1, 2}},
        {{0, 2}, {1, 3}},
        {{0, 3}, {1, 4}}
    };
    SegmentsSet layer(segments);
    layer.set_labels_types({ test_labels::circuits_layer_id });
    for (std::size_t idx = 0; idx < layer.size(); ++idx) {
        layer.set_label_value(test_labels::circuits_layer_id, layer[idx], 0);
    }

    SegmentsLayer segments_layer_with_neighbours = AreaAnalyzer::findSegmentsNeighbours(layer);

    for (std::size_t idx = 0; idx < segments_layer_with_neighbours.size(); ++idx) {
        std::cout << "Segment: " << segments_layer_with_neighbours[idx] << std::endl;
        label_data_type top = segments_layer_with_neighbours.get_label_value(1, segments_layer_with_neighbours[idx]);
        label_data_type bottom = segments_layer_with_neighbours.get_label_value(2, segments_layer_with_neighbours[idx]);
        if (top >= 0) {
            std::cout << "top: " << segments_layer_with_neighbours[top] << std::endl;
        } else {
            std::cout << "top neighbour not found" << std::endl;
        }
        if (bottom >= 0) {
            std::cout << "bottom: " << segments_layer_with_neighbours[bottom] << std::endl;
        } else {
            std::cout << "bottom neighbour not found" << std::endl;
        }
    }

    std::cout << std::endl << std::endl;

    SegmentsLayer segments_layer_with_marked_areas = AreaAnalyzer::markAreas(segments_layer_with_neighbours);

    for (std::size_t idx = 0; idx < segments_layer_with_marked_areas.size(); ++idx) {
        std::cout << "Segment: " << segments_layer_with_marked_areas[idx] << std::endl;
        std::cout << "Top area, first circuits layer: " << segments_layer_with_marked_areas.get_label_value(0, segments_layer_with_marked_areas[idx]) << std::endl;
        std::cout << "Top area, second circuits layer: " << segments_layer_with_marked_areas.get_label_value(1, segments_layer_with_marked_areas[idx]) << std::endl;
        std::cout << "Bottom area, first circuits layer: " << segments_layer_with_marked_areas.get_label_value(2, segments_layer_with_marked_areas[idx]) << std::endl;
        std::cout << "Bottom area, second circuits layer: " << segments_layer_with_marked_areas.get_label_value(3, segments_layer_with_marked_areas[idx]) << std::endl;
    }

    std::cout << std::endl;
}

void TestAreasDefault() {
    std::vector<Segment> segments = {
        {{1, 3}, {3, 5}},
        {{3, 5}, {6, 8}},
        {{6, 8}, {10, 8}},
        {{10, 8}, {11.5, 5.5}},
        {{11.5, 5.5}, {13, 3}},
        {{13, 3}, {9, 3}},
        {{9, 3}, {5, 3}},
        {{5, 3}, {1, 3}}, //
        {{1, 7}, {6, 12}},
        {{6, 12}, {10, 8}},
        {{10, 8}, {12, 6}},
        {{12, 6}, {11.5, 5.5}},
        {{11.5, 5.5}, {9, 3}},
        {{9, 3}, {7, 1}},
        {{7, 1}, {5, 3}},
        {{5, 3}, {3, 5}},
        {{3, 5}, {1, 7}}
    };

    SegmentsSet layer(segments);
    layer.set_labels_types({ test_labels::circuits_layer_id });
    for (std::size_t idx = 0; idx < 8; ++idx) {
        layer.set_label_value(test_labels::circuits_layer_id, layer[idx], 0);
    }
    for (std::size_t idx = 8; idx < layer.size(); ++idx) {
        layer.set_label_value(test_labels::circuits_layer_id, layer[idx], 1);
    }

    SegmentsLayer segments_layer_with_neighbours = AreaAnalyzer::findSegmentsNeighbours(layer);

    for (std::size_t idx = 0; idx < segments_layer_with_neighbours.size(); ++idx) {
        std::cout << "Segment: " << segments_layer_with_neighbours[idx] << std::endl;
        label_data_type top = segments_layer_with_neighbours.get_label_value(1, segments_layer_with_neighbours[idx]);
        label_data_type bottom = segments_layer_with_neighbours.get_label_value(2, segments_layer_with_neighbours[idx]);
        if (top >= 0) {
            std::cout << "top: " << segments_layer_with_neighbours[top] << std::endl;
        } else {
            std::cout << "top neighbour not found" << std::endl;
        }
        if (bottom >= 0) {
            std::cout << "bottom: " << segments_layer_with_neighbours[bottom] << std::endl;
        } else {
            std::cout << "bottom neighbour not found" << std::endl;
        }
        std::cout << std::endl;
    }
}

void TestAreasVert() {
    std::vector<Segment> input_segments = {
        {{2, 1}, {2, 4}},
        {{2, 4}, {2, 5}},
        {{2, 5}, {2, 7}},
        {{2, 7}, {4, 7}},
        {{4, 7}, {6, 7}},
        {{6, 7}, {8, 7}},
        {{8, 7}, {8, 5}},
        {{8, 5}, {8, 4}},
        {{8, 4}, {8, 1}},
        {{8, 1}, {2, 1}},
        {{1, 4}, {2, 5}},
        {{2, 5}, {4, 7}},
        {{4, 7}, {5, 8}},
        {{5, 8}, {6, 7}},
        {{6, 7}, {8, 5}},
        {{8, 5}, {9, 4}},
        {{9, 4}, {8, 4}},
        {{8, 4}, {2, 4}},
        {{2, 4}, {1, 4}}
    };


    SegmentsSet expected = input_segments;
    OutputSerializer::serializeSegmentsSet(input_segments, "result.txt");

    expected.set_labels_types({0, 1, 2});
    expected.set_label_values(0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1});
    expected.set_label_values(1, {8, 7, 11, -1, 12, -1, -1, 15, -1, 17, -1, 3, -1, -1, 5, -1, 15, 11, 10});
    expected.set_label_values(2, {-1, 10, -1, 11, 17, 14, 14, 1, 0, -1, 18, 17, 4, 4, 17, 16, -1, 9, -1});

    SegmentsSet test_layer = input_segments;
    test_layer.set_labels_types({0, 1, 2});
    test_layer.set_label_values(0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1});

    auto actual = AreaAnalyzer::findSegmentsNeighbours(test_layer);

    REQUIRE_EQ(actual.size(), expected.size());

    for (std::size_t idx = 0; idx < expected.size(); ++idx) {
        REQUIRE_EQ(actual.get_label_value(0, actual[idx]), expected.get_label_value(0, expected[idx]));
        REQUIRE_EQ(actual.get_label_value(1, actual[idx]), expected.get_label_value(1, expected[idx]));
        REQUIRE_EQ(actual.get_label_value(2, actual[idx]), expected.get_label_value(2, expected[idx]));
    }
}

void TestAreasFirstPhase() {
    std::vector<Segment> input_segments = {
        {{1, 7}, {6, 12}},
        {{3, 5}, {1, 7}},
        {{1, 3}, {3, 5}},
        {{5, 3}, {1, 3}},
        {{3, 5}, {6, 8}},
        {{5, 3}, {3, 5}},
        {{9, 3}, {5, 3}},
        {{7, 1}, {5, 3}},
        {{6, 12}, {10, 8}},
        {{6, 8}, {10, 8}},
        {{9, 3}, {7, 1}},
        {{11.5, 5.5}, {9, 3}},
        {{13, 3}, {9, 3}},
        {{10, 8}, {12, 6}},
        {{10, 8}, {11.5, 5.5}},
        {{12, 6}, {11.5, 5.5}},
        {{11.5, 5.5}, {13, 3}}
    };

    SegmentsSet expected = input_segments;

    expected.set_labels_types({0, 1, 2});
    expected.set_label_values(0, {1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0});
    expected.set_label_values(1, {-1, 0, 1, 2, 0, 4, 4, 6, -1, 8, 6, 9, 11, -1, 13, 13, 15});
    expected.set_label_values(2, {1, 2, 3, -1, 5, 3, 7, -1, 9, 6, -1, 12, -1, 14, 11, 16, 12});

    SegmentsSet test_layer = input_segments;
    test_layer.set_labels_types({0, 1, 2});
    test_layer.set_label_values(0, {1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0});

    auto actual = AreaAnalyzer::findSegmentsNeighbours(test_layer);

    REQUIRE_EQ(actual.size(), expected.size());

    for (std::size_t idx = 0; idx < expected.size(); ++idx) {
        REQUIRE_EQ(actual.get_label_value(0, actual[idx]), expected.get_label_value(0, expected[idx]));
        REQUIRE_EQ(actual.get_label_value(1, actual[idx]), expected.get_label_value(1, expected[idx]));
        REQUIRE_EQ(actual.get_label_value(2, actual[idx]), expected.get_label_value(2, expected[idx]));
    }
}

void TestAreasSecondPhase() {
    std::vector<Segment> input_segments = {
        {{1, 7}, {6, 12}},
        {{3, 5}, {1, 7}},
        {{1, 3}, {3, 5}},
        {{5, 3}, {1, 3}},
        {{3, 5}, {6, 8}},
        {{5, 3}, {3, 5}},
        {{9, 3}, {5, 3}},
        {{7, 1}, {5, 3}},
        {{6, 12}, {10, 8}},
        {{6, 8}, {10, 8}},
        {{9, 3}, {7, 1}},
        {{11.5, 5.5}, {9, 3}},
        {{13, 3}, {9, 3}},
        {{10, 8}, {12, 6}},
        {{10, 8}, {11.5, 5.5}},
        {{12, 6}, {11.5, 5.5}},
        {{11.5, 5.5}, {13, 3}}
    };

    SegmentsSet expected = input_segments;
    expected.set_labels_types({0, 1, 2, 3});

    expected.set_label_values(0, {0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0});
    expected.set_label_values(1, {0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0});
    expected.set_label_values(2, {0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1});
    expected.set_label_values(3, {1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0});

    SegmentsSet test_layer = input_segments;
    test_layer.set_labels_types({0, 1, 2});
    test_layer.set_label_values(0, {1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0});

    auto actual = AreaAnalyzer::findAreas(test_layer);

    REQUIRE_EQ(actual.size(), expected.size());

    for (std::size_t idx = 0; idx < actual.size(); ++idx) {
        REQUIRE_EQ(actual.get_label_value(0, actual[idx]), expected.get_label_value(0, expected[idx]));
        REQUIRE_EQ(actual.get_label_value(1, actual[idx]), expected.get_label_value(1, expected[idx]));
        REQUIRE_EQ(actual.get_label_value(2, actual[idx]), expected.get_label_value(2, expected[idx]));
        REQUIRE_EQ(actual.get_label_value(3, actual[idx]), expected.get_label_value(3, expected[idx]));
    }
}

void check_result(const SegmentsLayer& actual, const SegmentsLayer& expected) {
    REQUIRE_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < expected.size(); i++) {
        REQUIRE_EQ(actual.get_label_value(0, actual[i]), expected.get_label_value(0, expected[i]));
        REQUIRE_EQ(actual.get_label_value(1, actual[i]), expected.get_label_value(1, expected[i]));
        REQUIRE_EQ(actual.get_label_value(2, actual[i]), expected.get_label_value(2, expected[i]));
        REQUIRE_EQ(actual.get_label_value(3, actual[i]), expected.get_label_value(3, expected[i]));
    }
}

void TestAreasFirst() {
    std::vector<Segment> input_seg = {
        {{1, 2}, {6, 9}},
        {{4, 3}, {1, 2}},
        {{3, 0}, {4, 3}},
        {{11, 1}, {3, 0}},
        {{4, 3}, {5, 6}},
        {{10, 5}, {4, 3}},
        {{5, 6}, {8, 7}},
        {{6, 9}, {8, 7}},
        {{8, 7}, {14, 9}},
        {{8, 7}, {10, 5}},
        {{10, 5}, {14, 9}},
        {{11, 1}, {10, 5}},
        {{14, 9}, {11, 1}},
        {{17, 4}, {11, 1}},
        {{14, 9}, {15, 10}},
        {{15, 10}, {17, 4}}};

    SegmentsSet input_phase1 = input_seg;
    input_phase1.set_labels_types({0, 1, 2});
    input_phase1.set_label_values(0, {0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0});
    auto test_layer = AreaAnalyzer::findSegmentsNeighbours(input_phase1);

    SegmentsSet expected = input_seg;
    expected.set_labels_types({0, 1, 2, 3});

    expected.set_label_values(0, {0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0});
    expected.set_label_values(1, {0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0});
    expected.set_label_values(2, {1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1});
    expected.set_label_values(3, {0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0});

    auto actual = AreaAnalyzer::markAreas(test_layer);

    check_result(actual, expected);
}

void TestAreasSecond() {
    std::vector<Segment> input_seg = {
        {{2.5, 6}, {5.5, 6}},
        {{5, 5}, {2.5, 6}},
        {{4, 3}, {5, 5}},
        {{10, 3}, {4, 3}},
        {{5, 5}, {5.5, 6}},
        {{10, 3}, {5, 5}},
        {{5.5, 6}, {7, 9}},
        {{5.5, 6}, {8, 6}},
        {{7, 9}, {9.5, 9}},
        {{8, 13}, {16, 13}},
        {{10, 10}, {8, 13}},
        {{8, 6}, {9.5, 9}},
        {{12, 9}, {8, 6}},
        {{8, 6}, {13.5, 6}},
        {{9.5, 9}, {10, 10}},
        {{9.5, 9}, {12, 9}},
        {{10, 10}, {11, 12}},
        {{14, 12}, {10, 10}},
        {{14, 5}, {10, 3}},
        {{15, 3}, {10, 3}},
        {{11, 12}, {14, 12}},
        {{16, 12}, {12, 9}},
        {{12, 9}, {13.5, 6}},
        {{13.5, 6}, {16, 6}},
        {{13.5, 6}, {14, 5}},
        {{16, 13}, {14, 12}},
        {{14, 12}, {16, 12}},
        {{16, 6}, {14, 5}},
        {{14, 5}, {15, 3}}};

    SegmentsSet input_phase1 = input_seg;
    input_phase1.set_labels_types({0, 1, 2});
    input_phase1.set_label_values(0, {1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0});
    auto test_layer = AreaAnalyzer::findSegmentsNeighbours(input_phase1);

    SegmentsSet expected = input_seg;
    expected.set_labels_types({0, 1, 2, 3});

    expected.set_label_values(0, {0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0});
    expected.set_label_values(1, {0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0});
    expected.set_label_values(2, {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1});
    expected.set_label_values(3, {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0});


    auto actual = AreaAnalyzer::markAreas(test_layer);
    check_result(actual, expected);
}

DECLARE_TEST(TestAreasSimple);
DECLARE_TEST(TestAreasVert);
DECLARE_TEST(TestAreasFirstPhase);
DECLARE_TEST(TestAreasSecondPhase);
DECLARE_TEST(TestAreasFirst);
DECLARE_TEST(TestAreasSecond);
