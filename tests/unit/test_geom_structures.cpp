#include "test.hpp"
#include "gkernel/containers.hpp"

using namespace gkernel;

void PointTest() {
    Point p(1, 2);
    REQUIRE_EQ(p.x(), 1);
    REQUIRE_EQ(p.y(), 2);
}

void SegmentTest() {
    Point start(1, 2);
    Point end(3, 4);
    Segment s(start, end);

    REQUIRE_EQ(s.start(), start);
    REQUIRE_EQ(s.end(), end);
}

DECLARE_TEST(PointTest)
DECLARE_TEST(SegmentTest)

