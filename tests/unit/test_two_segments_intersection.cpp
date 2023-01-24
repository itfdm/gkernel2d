#include "test.hpp"

#include "gkernel/intersection.hpp"
#include "gkernel/objects.hpp"

using namespace gkernel;

void test_simple_intersection() {
    Segment s1({ {0, 2}, {2,0} });
    Segment s2({ {1, 0}, {1,2} });

    Point actual = Intersection::intersectSegments(s1, s2);

    REQUIRE_EQ(actual.x(), 1);
    REQUIRE_EQ(actual.y(), 1);
}

void test_end_point_intersection() {
    Segment s1({ {1, 0}, {1,2} });
    Segment s2({ {1, 2}, {3,0} });

    Point actual = Intersection::intersectSegments(s1, s2);

    REQUIRE_EQ(actual.x(), 1);
    REQUIRE_EQ(actual.y(), 2);
}

void test_orthogonal() {
    Segment s1({ {1, 0}, {1,2} });
    Segment s2({ {1, 2}, {3,2} });

    Point actual = Intersection::intersectSegments(s1, s2);

    REQUIRE_EQ(actual.x(), 1);
    REQUIRE_EQ(actual.y(), 2);
}

DECLARE_TEST(test_simple_intersection);
DECLARE_TEST(test_end_point_intersection);
DECLARE_TEST(test_orthogonal);
