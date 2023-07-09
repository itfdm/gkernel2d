#include "test.hpp"
#include "gkernel/intersection.hpp"
#define private public
#include "gkernel/converter.hpp"

using namespace gkernel;

void compare_result(const SegmentsLayer& segments_layer, const std::vector<Segment>& expected) {
    REQUIRE_EQ(segments_layer.size(), expected.size());

    for (const auto& seg : expected) {
        bool segment_finded = false;
        for (size_t i = 0; i < segments_layer.size(); ++i) {
            if (segments_layer[i] == seg) {
                segment_finded = true;
                break;
            }
        }
        REQUIRE_EQ(segment_finded, true);
    }
}

void simple_test() {
    std::vector<Segment> test_segments {
        {{0, 1}, {3, 1}},
        {{1, 0}, {1, 2}},
        {{1, 0}, {3, 2}}
    };
    SegmentsSet segments_set(test_segments);

    segments_set.set_labels_types({0});
    segments_set.set_label_values(0, {1, 2, 3});

    std::vector<IntersectionPoint> intersections {
        {{1, 1}, 0, 1},
        {{1, 0}, 1, 2},
        {{2, 1}, 0, 2}
    };

    std::vector<Segment> expected_segments {
        {{0, 1}, {1, 1}},
        {{1, 1}, {2, 1}},
        {{2, 1}, {3, 1}},
        {{1, 0}, {1, 1}},
        {{1, 1}, {1, 2}},
        {{1, 0}, {2, 1}},
        {{2, 1}, {3, 2}}
    };

    std::vector<label_data_type> expected_labels {
        1, 1, 1,
        2, 2,
        3, 3
    };

    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(segments_set, intersections);
    compare_result(segments_layer, expected_segments);

    REQUIRE(segments_set.get_label_types() == segments_layer.get_label_types());
    for (size_t i = 0; i < expected_segments.size(); ++i) {
        REQUIRE_EQ(segments_layer.get_label_value(0, segments_layer[i]), expected_labels[i]);
    }
}

void test_no_intersections() {
     std::vector<Segment> test_segments {
        {{1, 6}, {3, 6}},
        {{1, 3}, {4, 6}},
        {{1, 1}, {2, 2}},
        {{3, 4}, {6, 2}},
        {{6, 6}, {6, 3}}
    };
    SegmentsSet seg_set(test_segments);

    std::vector<IntersectionPoint> intersections;
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected {
        {{1, 6}, {3, 6}},
        {{1, 3}, {4, 6}},
        {{1, 1}, {2, 2}},
        {{3, 4}, {6, 2}},
        {{6, 6}, {6, 3}}
    };

    compare_result(segments_layer, expected);
}

void test_simple_intersections() {
    std::vector<Segment> test_segments {
        {{1, 5}, {10, 2}},
        {{2, 2}, {6, 6}},
        {{4, 6}, {9, 1}}
    };

    SegmentsSet seg_set(test_segments);
    std::vector<IntersectionPoint> intersections {
        {{4, 4}, 0, 1},
        {{7, 3}, 0, 2},
        {{5, 5}, 1, 2}
    };
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected {
        {{2, 2}, {4, 4}},
        {{4, 4}, {5, 5}},
        {{5, 5}, {6, 6}},
        {{1, 5}, {4, 4}},
        {{4, 4}, {7, 3}},
        {{7, 3}, {10, 2}},
        {{4, 6}, {5, 5}},
        {{5, 5}, {7, 3}},
        {{7, 3}, {9, 1}}
    };

    compare_result(segments_layer, expected);
}

void test_intersections_in_end_points() {
    std::vector<Segment> test_segments {
        {{1, 1}, {2, 6}},   // 0
        {{1, 1}, {7, 3}},   // 1
        {{2, 6}, {7, 3}},   // 2
        {{2, 6}, {4, 2}}    // 3
    };

    SegmentsSet seg_set(test_segments);
    std::vector<IntersectionPoint> intersections{
        {{1, 1}, 0, 1},
        {{2, 6}, 0, 2},
        {{2, 6}, 0, 3},
        {{2, 6}, 2, 3},
        {{7, 3}, 1, 2},
        {{4, 2}, 1, 3}
    };
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected {
        {{1, 1}, {2, 6}},
        {{2, 6}, {7, 3}},
        {{2, 6}, {4, 2}},
        {{1, 1}, {4, 2}},
        {{4, 2}, {7, 3}}
    };

    compare_result(segments_layer, expected);
}

void test_overlapping() {
    std::vector<Segment> test_segments {
        {{1, 1}, {6, 6}},   // 0
        {{1, 4}, {5, 2}},   // 1
        {{1, 1}, {4, 4}},   // 2
        {{5, 5}, {7, 7}}    // 3
    };

    SegmentsSet seg_set(test_segments);

    // just to make it compile, uncomment the tuple version when overlap is supported
    std::vector<IntersectionPoint> intersections {
        {{3, 3}, 0, 1},
        {{1, 1}, 0, 2},
        {{3, 3}, 1, 2},
        {{5, 5}, 0, 3}
    };
    // std::vector<std::tuple<Segment, segment_id, segment_id>> intersections {
    //     {{{3, 3}, {3, 3}}, 0, 1},
    //     {{{1, 1}, {4, 4}}, 0, 2},
    //     {{{3, 3}, {3, 3}}, 1, 2},
    //     {{{5, 5}, {6, 6}}, 0, 3}
    // };
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected {
        {{1, 1}, {3, 3}},
        {{3, 3}, {4, 4}},
        {{4, 4}, {5, 5}},
        {{5, 5}, {6, 6}},
        {{6, 6}, {7, 7}},
        {{1, 4}, {3, 3}},
        {{3, 3}, {5, 2}}
    };

    compare_result(segments_layer, expected);
}

void test_full_overlapping() {
    std::vector<Segment> test_segments {
        {{1, 1}, {4, 4}},   // 0
        {{1, 1}, {7, 1}},   // 1
        {{4, 4}, {7, 1}},   // 2

        {{1, 1}, {4, 4}},   // 3
        {{1, 1}, {7, 1}},   // 4
        {{4, 4}, {7, 1}},   // 5
    };
    SegmentsSet seg_set(test_segments);

    std::vector<IntersectionPoint> intersections {
        {{1, 1}, 0, 1},
        {{4, 4}, 0, 2},
        {{7, 1}, 1, 2},

        {{1, 1}, 3, 4},
        {{4, 4}, 3, 5},
        {{7, 1}, 4, 5},

        {{1, 1}, 0, 4},
        {{4, 4}, 0, 5},
        {{7, 1}, 1, 5},

        {{1, 1}, 0, 3},
        {{1, 1}, 1, 4},
        {{4, 4}, 2, 5}
    };

    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected {
        {{1, 1}, {4, 4}},
        {{1, 1}, {7, 1}},
        {{4, 4}, {7, 1}}
    };

    compare_result(segments_layer, expected);
}

void test_star() {
    std::vector<Segment> test_segments {
        {{1, 3}, {7, 3}},   // 0
        {{2, 5}, {6, 1}},   // 1
        {{2, 1}, {6, 5}},   // 2
    };

    SegmentsSet seg_set(test_segments);
    std::vector<IntersectionPoint> intersections {
        {{4, 3}, 0, 1},
        {{4, 3}, 0, 2},
        {{4, 3}, 1, 2}
    };
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected {
        {{1, 3}, {4, 3}},
        {{2, 5}, {4, 3}},
        {{6, 5}, {4, 3}},
        {{7, 3}, {4, 3}},
        {{6, 1}, {4, 3}},
        {{2, 1}, {4, 3}}
    };

    compare_result(segments_layer, expected);
}

void test_orthogonal() {
    std::vector<Segment> test_segments {
        {{1, 2}, {5, 2}},   // 0
        {{3, 1}, {3, 4}},   // 1
        {{3, 4}, {6, 4}},   // 2
        {{5, 2}, {5, 5}},   // 3
    };

    SegmentsSet seg_set(test_segments);
    std::vector<IntersectionPoint> intersections {
        {{3, 2}, 0, 1},
        {{3, 4}, 1, 2},
        {{5, 2}, 0, 3},
        {{5, 4}, 2, 3}
    };
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected {
        {{1, 2}, {3, 2}},
        {{3, 2}, {5, 2}},
        {{3, 4}, {5, 4}},
        {{5, 4}, {6, 4}},
        {{3, 2}, {3, 1}},
        {{3, 2}, {3, 4}},
        {{5, 2}, {5, 4}},
        {{5, 4}, {5, 5}}
    };

    compare_result(segments_layer, expected);
}

void test_hard() {
    std::vector<Segment> test_segments {
        {{0, 0}, {4, 4}}, // 0
        {{4, 4}, {8, 0}}, // 1
        {{0, 0}, {8, 0}}, // 2
        {{2, 4}, {4, 2}}, // 3
        {{2, 4}, {6, 4}}, // 4
        {{4, 2}, {6, 4}}, // 5
        {{3, 0}, {3, 5}}, // 6
        {{3, 5}, {8, 5}}, // 7
        {{8, 0}, {8, 5}}, // 8
        {{4, 1}, {4, 7}}, // 9
        {{4, 7}, {6, 7}}, // 10
        {{6, 1}, {6, 7}}, // 11
        {{4, 1}, {6, 1}}, // 12
    };

    SegmentsSet seg_set(test_segments);
    std::vector<IntersectionPoint> intersections {
        {{3, 3}, 0, 3},
        {{3, 3}, 0, 6},
        {{3, 3}, 3, 6},

        {{4, 2}, 3, 5},
        {{4, 2}, 3, 9},
        {{4, 2}, 5, 9},

        {{4, 4}, 0, 1},
        {{4, 4}, 0, 4},
        {{4, 4}, 0, 9},
        {{4, 4}, 1, 4},
        {{4, 4}, 1, 9},
        {{4, 4}, 4, 9},

        {{6, 4}, 4, 5},
        {{6, 4}, 4, 11},
        {{6, 4}, 5, 11},

        {{8, 0}, 1, 2},
        {{8, 0}, 1, 8},
        {{8, 0}, 2, 8},

        {{0, 0}, 0, 2},
        {{2, 4}, 3, 4},
        {{3, 0}, 2, 6},
        {{3, 4}, 4, 6},
        {{3, 5}, 6, 7},
        {{4, 1}, 9, 12},
        {{4, 5}, 7, 9},
        {{4, 7}, 9, 10},
        {{5, 3}, 1, 5},
        {{6, 1}, 11, 12},
        {{6, 2}, 1, 11},
        {{6, 5}, 7, 11},
        {{6, 7}, 10, 11},
        {{8, 5}, 7, 8},
    };
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected {
        {{0, 0}, {3, 3}},
        {{3, 3}, {4, 4}},
        {{4, 4}, {5, 3}},
        {{5, 3}, {6, 2}},
        {{6, 2}, {8, 0}},
        {{0, 0}, {3, 0}},
        {{3, 0}, {8, 0}},
        {{3, 0}, {3, 3}},
        {{3, 3}, {3, 4}},
        {{3, 4}, {3, 5}},
        {{3, 5}, {4, 5}},
        {{4, 5}, {6, 5}},
        {{6, 5}, {8, 5}},
        {{8, 5}, {8, 0}},
        {{4, 2}, {3, 3}},
        {{3, 3}, {2, 4}},
        {{2, 4}, {3, 4}},
        {{3, 4}, {4, 4}},
        {{4, 4}, {6, 4}},
        {{6, 4}, {5, 3}},
        {{5, 3}, {4, 2}},
        {{4, 1}, {4, 2}},
        {{4, 2}, {4, 4}},
        {{4, 4}, {4, 5}},
        {{4, 5}, {4, 7}},
        {{4, 7}, {6, 7}},
        {{6, 7}, {6, 5}},
        {{6, 5}, {6, 4}},
        {{6, 4}, {6, 2}},
        {{6, 2}, {6, 1}},
        {{6, 1}, {4, 1}} };

    compare_result(segments_layer, expected);
}

#define DECLARE_TEST(TestName) TEST_CASE(#TestName) { TestName(); }

DECLARE_TEST(simple_test)
DECLARE_TEST(test_no_intersections)
DECLARE_TEST(test_simple_intersections)
DECLARE_TEST(test_intersections_in_end_points)
// DECLARE_TEST(test_overlapping) // overlapping segments are not supported
// DECLARE_TEST(test_full_overlapping) // overlapping segments are not supported
DECLARE_TEST(test_star)
DECLARE_TEST(test_orthogonal)
DECLARE_TEST(test_hard)
