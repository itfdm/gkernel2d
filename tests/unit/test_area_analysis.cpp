#include "test.hpp"
#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"
#include <limits>

using namespace gkernel;

void check_result(const SegmentsLayer &actual, const SegmentsLayer &expected)
{
    REQUIRE_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < expected.size(); i++)
    {
        REQUIRE_EQ(actual.get_label_value(0, actual[i]), expected.get_label_value(1, expected[i]));
        REQUIRE_EQ(actual.get_label_value(1, actual[i]), expected.get_label_value(2, expected[i]));
        REQUIRE_EQ(actual.get_label_value(2, actual[i]), expected.get_label_value(3, expected[i]));
        REQUIRE_EQ(actual.get_label_value(3, actual[i]), expected.get_label_value(4, expected[i]));
    }
}

void test_phase_1_1()
{
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
        {{11.5, 5.5}, {13, 3}}};

    SegmentsLayer expected = input_segments;

    /* метка 0 - номер слоя
     * метка 1 - id соседа сверху
     * метка 2 - id соседа снизу
     */

    expected.set_labels_types({0, 1, 2});
    expected.set_label_values(0, {1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0});
    expected.set_label_values(1, {-1, 0, 1, 2, 0, 4, 4, 6, -1, 8, 6, 9, 11, -1, 13, 13, 15});
    expected.set_label_values(2, {1, 2, 3, -1, 5, 3, 7, -1, 9, 6, -1, 12, -1, 14, 11, 16, 12});

    SegmentsLayer test_layer = input_segments;
    test_layer.set_labels_types({0, 1, 2});
    test_layer.set_label_values(0, {1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0});

    auto actual = AreaAnalysis::findSegmentsNeighbours(test_layer);

    REQUIRE_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < expected.size(); i++)
    {
        REQUIRE_EQ(actual.get_label_value(0, actual[i]), expected.get_label_value(0, expected[i]));
        REQUIRE_EQ(actual.get_label_value(1, actual[i]), expected.get_label_value(1, expected[i]));
        REQUIRE_EQ(actual.get_label_value(2, actual[i]), expected.get_label_value(2, expected[i]));
    }
}

void test_phase_1_2()
{
    std::vector<Segment> input_segments = {
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

    SegmentsLayer expected = input_segments;

    expected.set_labels_types({0, 1, 2});
    expected.set_label_values(0, {0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0});
    expected.set_label_values(1, {-1, 0, 1, 2, 0, 4, 0, -1, -1, 8, 8, 10, 10, 12, -1, -1});
    expected.set_label_values(2, {1, -1, 3, -1, 5, 3, 5, 6, 9, 5, 11, 3, 13, -1, 13, 13});

    SegmentsLayer test_layer = input_segments;
    test_layer.set_labels_types({0, 1, 2});
    test_layer.set_label_values(0, {0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0});

    auto actual = AreaAnalysis::findSegmentsNeighbours(test_layer);

    REQUIRE_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < expected.size(); i++)
    {
        REQUIRE_EQ(actual.get_label_value(0, actual[i]), expected.get_label_value(0, expected[i]));
        REQUIRE_EQ(actual.get_label_value(1, actual[i]), expected.get_label_value(1, expected[i]));
        REQUIRE_EQ(actual.get_label_value(2, actual[i]), expected.get_label_value(2, expected[i]));
    }
}

void test_phase_1_3()
{
    std::vector<Segment> input_segments = {
        {{2.5, 6}, {5.5, 6}},
        {{5, 5}, {2.5, 6}},
        {{4, 3}, {5, 3}},
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

    SegmentsLayer expected = input_segments;

    expected.set_labels_types({0, 1, 2});
    expected.set_label_values(0, {1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0});
    expected.set_label_values(1, {-1, 0, -1, 2, 0, 4, -1, 6, -1, 0, 9, 8, 11, 12, 10, 14, 9, 16, 13, 18, 9, 17, 21, 21, 23, 9, 25, 23, 27});
    expected.set_label_values(2, {1, -1, 3, -1, 5, 3, 7, 5, 7, 10, 8, 12, 13, 5, 15, 12, 17, 15, 19, -1, 17, 22, 13, 24, 18, 26, 21, 28, 19});

    SegmentsLayer test_layer = input_segments;
    test_layer.set_labels_types({0, 1, 2});
    test_layer.set_label_values(0, {1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0});

    auto actual = AreaAnalysis::findSegmentsNeighbours(test_layer);

    REQUIRE_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < expected.size(); i++)
    {
        REQUIRE_EQ(actual.get_label_value(0, actual[i]), expected.get_label_value(0, expected[i]));
        REQUIRE_EQ(actual.get_label_value(1, actual[i]), expected.get_label_value(1, expected[i]));
        REQUIRE_EQ(actual.get_label_value(2, actual[i]), expected.get_label_value(2, expected[i]));
    }
}

void test_phase_1_4()
{
    std::vector<Segment> input_segments = {
        {{1, 8}, {2, 8}},
        {{1, 3}, {1, 8}},
        {{2, 3}, {1, 3}},
        {{2, 11}, {12, 11}},
        {{2, 8}, {2, 11}},
        {{2, 8}, {8, 8}},
        {{2, 3}, {2, 8}},
        {{8, 3}, {2, 3}},
        {{2, 1}, {2, 3}},
        {{12, 1}, {2, 1}},
        {{8, 8}, {8, 3}},
        {{9, 10}, {11, 10}},
        {{9, 8}, {9, 10}},
        {{11, 8}, {9, 8}},
        {{9, 6}, {12, 6}},
        {{9, 3}, {9, 6}},
        {{12, 3}, {9, 3}},
        {{11, 10}, {11, 8}},
        {{12, 11}, {12, 6}},
        {{12, 6}, {15, 6}},
        {{12, 6}, {12, 3}},
        {{15, 3}, {12, 3}},
        {{12, 3}, {12, 1}},
        {{15, 6}, {15, 3}}};

    SegmentsLayer expected = input_segments;

    expected.set_labels_types({0, 1, 2});
    expected.set_label_values(0, {1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1});
    expected.set_label_values(1, {-1, 0, 1, -1, 3, 4, 5, 6, 7, 8, 5, 3, 11, 12, 13, 14, 15, 11, 3, 18, 19, 20, 21, 19});
    expected.set_label_values(2, {1, 2, -1, 4, 5, 6, 7, 8, 9, -1, 9, 12, 13, 14, 15, 16, 9, 14, 19, 20, 21, 22, 9, 21});

    SegmentsLayer test_layer = input_segments;
    test_layer.set_labels_types({0, 1, 2});
    test_layer.set_label_values(0, {1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1});

    auto actual = AreaAnalysis::findSegmentsNeighbours(test_layer);

    REQUIRE_EQ(actual.size(), expected.size());

    for (size_t i = 0; i < expected.size(); i++)
    {
        REQUIRE_EQ(actual.get_label_value(0, actual[i]), expected.get_label_value(0, expected[i]));
        REQUIRE_EQ(actual.get_label_value(1, actual[i]), expected.get_label_value(1, expected[i]));
        REQUIRE_EQ(actual.get_label_value(2, actual[i]), expected.get_label_value(2, expected[i]));
    }
}

void test_1()
{
    std::vector<Segment> input_seg = {
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
        {{11.5, 5.5}, {13, 3}}};

    SegmentsLayer input_phase1 = input_seg;
    input_phase1.set_labels_types({0, 1, 2});
    input_phase1.set_label_values(0, {1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0});
    auto test_layer = AreaAnalysis::findSegmentsNeighbours(input_phase1);

    SegmentsLayer expected = input_seg;
    expected.set_labels_types({0, 1, 2, 3});

    expected.set_label_values(0, {0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0});
    expected.set_label_values(1, {0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0});
    expected.set_label_values(2, {0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1});
    expected.set_label_values(3, {1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0});

    auto actual = markAreas(test_layer);

    check_result(actual, expected);
}

void test_2()
{
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

    SegmentsLayer input_phase1 = input_seg;
    input_phase1.set_labels_types({0, 1, 2});
    input_phase1.set_label_values(0, {0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0});
    auto test_layer = AreaAnalysis::findSegmentsNeighbours(input_phase1);

    SegmentsLayer expected = input_seg;
    expected.set_labels_types({0, 1, 2, 3});

    expected.set_label_values(0, {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0});
    expected.set_label_values(1, {0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0});
    expected.set_label_values(2, {1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1});
    expected.set_label_values(3, {0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0});

    auto actual = AreaAnalysis::markAreas(test_layer);

    check_result(actual, expected);
}

void test_3()
{
    std::vector<Segment> input_seg = {
        {{2.5, 6}, {5.5, 6}},
        {{5, 5}, {2.5, 6}},
        {{4, 3}, {5, 3}},
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

    SegmentsLayer input_phase1 = input_seg;
    input_phase1.set_labels_types({0, 1, 2});
    input_phase1.set_label_values(0, {1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0});
    auto test_layer = AreaAnalysis::findSegmentsNeighbours(input_phase1);

    SegmentsLayer expected = input_seg;
    expected.set_labels_types({0, 1, 2, 3});

    expected.set_label_values(0, {0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0});
    expected.set_label_values(1, {0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0});
    expected.set_label_values(2, {0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1});
    expected.set_label_values(3, {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0});

    auto actual = AreaAnalysis::markAreas(test_layer);
    check_result(actual, expected);
}

void test_4()
{
    std::vector<Segment> input_seg = {
        {{1, 8}, {2, 8}},
        {{1, 3}, {1, 8}},
        {{2, 3}, {1, 3}},
        {{2, 11}, {12, 11}},
        {{2, 8}, {2, 11}},
        {{2, 8}, {8, 8}},
        {{2, 3}, {2, 8}},
        {{8, 3}, {2, 3}},
        {{2, 1}, {2, 3}},
        {{12, 1}, {2, 1}},
        {{8, 8}, {8, 3}},
        {{9, 10}, {11, 10}},
        {{9, 8}, {9, 10}},
        {{11, 8}, {9, 8}},
        {{9, 6}, {12, 6}},
        {{9, 3}, {9, 6}},
        {{12, 3}, {9, 3}},
        {{11, 10}, {11, 8}},
        {{12, 11}, {12, 6}},
        {{12, 6}, {15, 6}},
        {{12, 6}, {12, 3}},
        {{15, 3}, {12, 3}},
        {{12, 3}, {12, 1}},
        {{15, 6}, {15, 3}}};

    SegmentsLayer input_phase1 = input_seg;
    input_phase1.set_labels_types({0, 1, 2});
    input_phase1.set_label_values(0, {1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1});
    auto test_layer = AreaAnalysis::findSegmentsNeighbours(input_phase1);

    SegmentsLayer expected = input_seg;
    expected.set_labels_types({0, 1, 2, 3});

    expected.set_label_values(0, {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0});
    expected.set_label_values(1, {0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1});
    expected.set_label_values(2, {0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0});
    expected.set_label_values(3, {1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1});

    auto actual = AreaAnalysis::markAreas(test_layer);
    check_result(actual, expected);
}

DECLARE_TEST(test_phase_1_1);
DECLARE_TEST(test_phase_1_2);
DECLARE_TEST(test_phase_1_3);
DECLARE_TEST(test_phase_1_4);
DECLARE_TEST(test_1);
DECLARE_TEST(test_2);
DECLARE_TEST(test_3);
DECLARE_TEST(test_4);