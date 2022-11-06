#include "test.hpp"
#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"

using namespace gkernel;

enum TestLabels : label_type {
    RESERVED,
    FIRST_LABEL,
    SECOND_LABEL,
    THIRD_LABEL
};

std::vector<Segment> GenerateSegments(size_t size) {
    std::vector<Segment> segments;
    segments.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        data_type offset = static_cast<data_type>(4 * i);

        Point p1(offset,     offset + 1);
        Point p2(offset + 2, offset + 3);

        segments.emplace_back(Segment(p1, p2));
    }
    return segments;
}

void PointTest() {
    Point p(1, 2);
    REQUIRE_EQ(p.x(), 1);
    REQUIRE_EQ(p.y(), 2);
}

void SegmentTest() {
    Point begin_point(1, 2);
    Point end_point(3, 4);
    Segment s(begin_point, end_point);

    REQUIRE_EQ(s.begin_point(), begin_point);
    REQUIRE_EQ(s.end_point(), end_point);
}

void SegmentsSetAddingElementsTest() {
    std::vector<Segment> segments = GenerateSegments(2);
    SegmentsSet segments_set(segments);

    REQUIRE_EQ(segments_set.get_segment(0).begin_point(), Point(0, 1));
    REQUIRE_EQ(segments_set.get_segment(1).end_point(), Point(6, 7));

    segments_set.emplace_back(GenerateSegments(1).front());
    REQUIRE_EQ(segments_set.get_segment(2).end_point(), Point(2, 3));
}

void SegmentsSetlabels() {
    std::vector<Segment> segments = GenerateSegments(2);
    SegmentsSet segments_set(segments);

    REQUIRE_EQ(segments_set.has_label(TestLabels::FIRST_LABEL), false);

    REQUIRE_THROWS(segments_set.set_labels_types({TestLabels::RESERVED}));

    std::vector<label_type> label_types = { TestLabels::FIRST_LABEL, TestLabels::SECOND_LABEL };
    segments_set.set_labels_types(label_types);

    REQUIRE_EQ(segments_set.has_label(TestLabels::FIRST_LABEL), true);
    REQUIRE_EQ(segments_set.has_label(TestLabels::SECOND_LABEL), true);
    REQUIRE_EQ(segments_set.has_label(TestLabels::THIRD_LABEL), false);

    REQUIRE_THROWS(segments_set.set_labels_types(label_types));

    std::vector<label_data_type> label_values = { 13, 33 };
    segments_set.set_label_values(TestLabels::SECOND_LABEL, label_values);
    REQUIRE_EQ(segments_set.get_label_value(TestLabels::SECOND_LABEL, 1), 33);

    segments_set.set_label_value(TestLabels::SECOND_LABEL, 1, 37);
    REQUIRE_EQ(segments_set.get_label_value(TestLabels::SECOND_LABEL, 1), 37);
}

void VertexChainValidationTest() {
    std::vector<Segment> segments = GenerateSegments(2);
    REQUIRE_THROWS(VertexChain(segments));

    VertexChain vertex_chain;
    REQUIRE_NOTHROW(vertex_chain.emplace_back(segments[0]));
    REQUIRE_THROWS(vertex_chain.emplace_back(segments[1]));

    Segment segment({2, 3}, {4, 5});
    REQUIRE_NOTHROW(vertex_chain.emplace_back(segment));
}

void CircuitValidationTest() {
    std::vector<Segment> true_segments = {
        {{0, 1}, {2, 3}},
        {{2, 3}, {3, 4}},
        {{3, 4}, {0, 1}}
    };
    REQUIRE_NOTHROW(Circuit(true_segments));

    std::vector<Segment> false_segments = {
        {{0, 1}, {2, 3}},
        {{2, 3}, {3, 4}},
        {{3, 4}, {4, 5}}
    };
    REQUIRE_THROWS(Circuit(false_segments));
}

void CircuitsSetTest() {
    std::vector<Circuit> test_circuits = {
        Circuit(GenerateSegments(3), false),
        Circuit(GenerateSegments(4), false),
        Circuit(GenerateSegments(5), false)
    };
    CircuitsSet circuits_set(test_circuits);

    REQUIRE_EQ(circuits_set.get_circuit(1) == Circuit(GenerateSegments(4), false), true);

    circuits_set.emplace_back(test_circuits[0]);
    REQUIRE_EQ(circuits_set.size(), 4);
    REQUIRE_EQ(circuits_set.get_circuit(3) == Circuit(GenerateSegments(3), false), true);
}

#define DECLARE_TEST(TestName) TEST_CASE(#TestName) { TestName(); }

DECLARE_TEST(PointTest)
DECLARE_TEST(SegmentTest)
DECLARE_TEST(SegmentsSetAddingElementsTest)
DECLARE_TEST(SegmentsSetlabels)
DECLARE_TEST(VertexChainValidationTest)
DECLARE_TEST(CircuitValidationTest)
DECLARE_TEST(CircuitsSetTest)
