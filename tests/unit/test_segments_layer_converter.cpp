#include "test.hpp"
#include "gkernel/converter.hpp"
#include "gkernel/intersection.hpp"

using namespace gkernel;

void check_result(const SegmentsLayer& segments_layer, const std::vector<Segment>& expected){
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

void test_no_intersections(){
     std::vector<Segment> test_segments = {
        {{1, 6}, {3, 6}},
        {{1, 3}, {4, 6}},
        {{1, 1}, {2, 2}},
        {{3, 4}, {6, 2}},
        {{6, 6}, {6, 3}}
    };
    SegmentsSet seg_set(test_segments);

    std::vector<std::tuple<Segment, segment_id, segment_id>> intersections;
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected={
        {{1, 6}, {3, 6}},
        {{1, 3}, {4, 6}},
        {{1, 1}, {2, 2}},
        {{3, 4}, {6, 2}},
        {{6, 6}, {6, 3}}
    };

    check_result(segments_layer, expected);
}

void test_simple_intersections()
{
    std::vector<Segment> test_segments = {
        {{1, 5}, {10, 2}},
        {{2, 2}, {6, 6}},
        {{4, 6}, {9, 1}}
    };

    SegmentsSet seg_set(test_segments);
    std::vector<std::tuple<Segment, segment_id, segment_id>> intersections;
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections)={
        {{{4, 4}, {4, 4}}, 0, 2},
        {{{7, 3}, {7, 3}}, 0, 1},
        {{{5, 5}, {5, 5}}, 1, 2}
    };

    std::vector<Segment> expected={
        {{2,2}, {4, 4}},
        {{4, 4}, {5, 5}},
        {{5, 5}, {6, 6}},
        {{1, 5}, {4, 4}},
        {{4, 4}, {7, 3}},
        {{7, 3}, {10, 2}},
        {{4, 6}, {5, 5}},
        {{5, 5}, {7, 3}},
        {{7, 3}, {9, 1}}
    };

    check_result(segments_layer, expected);
}

void test_intersections_in_end_points()
{
    std::vector<Segment> test_segments = {
        {{1, 1}, {2, 6}},   // 0
        {{1, 1}, {7, 3}},   // 1
        {{2, 6}, {7, 3}},   // 2
        {{2, 6}, {4, 2}}    // 3
    };

    SegmentsSet seg_set(test_segments);
    std::vector<std::tuple<Segment, segment_id, segment_id>> intersections={
        {{{1, 1}, {1, 1}}, 0, 1},
        {{{2, 6}, {2, 6}}, 0, 2},
        {{{2, 6}, {2, 6}}, 0, 3},
        {{{2, 6}, {2, 6}}, 2, 3},
        {{{7, 3}, {7, 3}}, 1, 2},
        {{{4, 2}, {4, 2}}, 1, 3}
    };
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected={
        {{1, 1}, {2, 6}},
        {{2, 6}, {7, 3}},
        {{2, 6}, {4, 2}},
        {{1, 1}, {4, 2}},
        {{4, 2}, {7, 3}}
    };

    check_result(segments_layer, expected);
}

void test_overlapping()
{
    std::vector<Segment> test_segments = {
        {{1, 1}, {6, 6}},   // 0
        {{1, 4}, {5, 2}},   // 1
        {{1, 1}, {4, 4}},   // 2
        {{5, 5}, {7, 7}}    // 3
    };

    SegmentsSet seg_set(test_segments);
    std::vector<std::tuple<Segment, segment_id, segment_id>> intersections={
        {{{3, 3}, {3, 3}}, 0, 1},
        {{{1, 1}, {4, 4}}, 0, 2},
        {{{3, 3}, {3, 3}}, 1, 2},
        {{{5, 5}, {6, 6}}, 0, 3}
    };
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected={
        {{1, 1}, {3, 3}},
        {{3, 3}, {4, 4}},
        {{4, 4}, {5, 5}},
        {{5, 5}, {6, 6}},
        {{6, 6}, {7, 7}},
        {{1, 4}, {3, 3}},
        {{3, 3}, {5, 2}}
    };

    check_result(segments_layer, expected);
}

void test_full_overlapping()
{
    std::vector<Segment> test_segments = {
        {{1, 1}, {4, 4}},   // 0
        {{1, 1}, {7, 1}},   // 1
        {{4, 4}, {7, 1}},   // 2

        {{1, 1}, {4, 4}},   // 3
        {{1, 1}, {7, 1}},   // 4
        {{4, 4}, {7, 1}},   // 5
    };
    SegmentsSet seg_set(test_segments);
    
    std::vector<std::tuple<Segment, segment_id, segment_id>> intersections={
        {{{1, 1}, {1, 1}}, 0, 1},
        {{{4, 4}, {4, 4}}, 0, 2},
        {{{7, 1}, {7, 1}}, 1, 2},

        {{{1, 1}, {1, 1}}, 3, 4},
        {{{4, 4}, {4, 4}}, 3, 5},
        {{{7, 1}, {7, 1}}, 4, 5},

        {{{1, 1}, {7, 1}}, 0, 4},
        {{{4, 4}, {4, 4}}, 0, 5},
        {{{7, 1}, {7, 1}}, 1, 5},

        {{{1, 1}, {4, 1}}, 0, 3},
        {{{1, 1}, {7, 1}}, 1, 4},
        {{{4, 4}, {7, 1}}, 2, 5}
    };

    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected={
        {{1, 1}, {4, 4}},
        {{1, 1}, {7, 1}},
        {{4, 4}, {7, 1}}
    };

    check_result(segments_layer, expected);  
}

void test_star(){
    std::vector<Segment> test_segments = {
        {{1, 3}, {7, 3}},   // 0
        {{2, 5}, {6, 1}},   // 1
        {{2, 1}, {6, 5}},   // 2
    };

    SegmentsSet seg_set(test_segments);
    std::vector<std::tuple<Segment, segment_id, segment_id>> intersections={
        {{{4, 3}, {4, 3}}, 0, 1},
        {{{4, 3}, {4, 3}}, 0, 2},
        {{{4, 3}, {4, 3}}, 1, 2}
    };
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected={
        {{1, 3}, {4, 3}},
        {{2, 5}, {4, 3}},
        {{6, 5}, {4, 3}},
        {{7, 3}, {4, 3}},
        {{6, 1}, {4, 3}},
        {{2, 1}, {4, 3}}
    };

    check_result(segments_layer, expected);
}

void test_orthogonal(){
    std::vector<Segment> test_segments = {
        {{1, 2}, {5, 2}},   // 0
        {{3, 1}, {3, 4}},   // 1
        {{3, 4}, {6, 4}},   // 2
        {{5, 2}, {5, 5}},   // 3
    };

    SegmentsSet seg_set(test_segments);
    std::vector<std::tuple<Segment, segment_id, segment_id>> intersections={
        {{{3, 2}, {3, 2}}, 0, 1},
        {{{3, 4}, {3, 4}}, 1, 2},
        {{{5, 2}, {5, 2}}, 0, 3},
        {{{5, 4}, {5, 4}}, 2, 3}
    };
    SegmentsLayer segments_layer = Converter::convertToSegmentsLayer(seg_set, intersections);

    std::vector<Segment> expected={
        {{1, 2}, {3, 2}},
        {{3, 2}, {5, 2}},
        {{3, 4}, {5, 4}},
        {{5, 4}, {6, 4}},
        {{3, 2}, {3, 1}},
        {{3, 2}, {3, 4}},
        {{5, 2}, {5, 4}},
        {{5, 4}, {5, 5}}
    };

    check_result(segments_layer, expected);
}

TEST_CASE("no insertions")
{
    test_no_intersections();
}

TEST_CASE("Test simple")
{
    test_simple_intersections();
}

TEST_CASE("Test end points")
{
    test_intersections_in_end_points();
}

TEST_CASE("Test partial overlapping")
{
    test_overlapping();
}

TEST_CASE("Test full overlapping")
{
    test_full_overlapping();
}

TEST_CASE("Test star")
{
    test_star();
}

TEST_CASE("Test orthogonal")
{
    test_orthogonal();
}