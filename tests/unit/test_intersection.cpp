#include "test.hpp"

#include <gkernel/intersection.hpp>
#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"

#include <unordered_set>
#include <algorithm>

using namespace gkernel;

template <typename T>
class SegmentHasher {
public:
    std::size_t operator()(const T& segment) const {
        return segment.hash();
    }
};

void run_intersect_segments_test(gkernel::SegmentsSet& input, std::vector<gkernel::Point>& expected) {
    auto result = Intersection::intersectSetSegments(input);
    std::vector<gkernel::Point> result_points;

    for (auto& point : result) {
        result_points.emplace_back(point.point());
    }

    std::sort(expected.begin(), expected.end(), [](const gkernel::Point& first, const gkernel::Point& second) {
        if (first.x() != second.x()) {
            return first.x() < second.x();
        } else {
            return first.y() < second.y();
        }
    });

    REQUIRE_EQ(std::equal(result_points.begin(), result_points.end(), expected.begin(), expected.end(), [](const gkernel::Point& first, const gkernel::Point& second) {
        return first.x() == second.x() && first.y() == second.y();
    }), true);
}

void TestSegmentsSetIntersectionFirst() {
    gkernel::SegmentsSet input;
    input.emplace_back({gkernel::Point(9, 6), gkernel::Point(10, 5)});
    input.emplace_back({gkernel::Point(6, 1), gkernel::Point(10, 5)});
    input.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(4, 1)});
    input.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(6, 3)});
    input.emplace_back({gkernel::Point(4, 3), gkernel::Point(6, 1)});
    input.emplace_back({gkernel::Point(4, 3), gkernel::Point(6, 5)});
    input.emplace_back({gkernel::Point(4, 1), gkernel::Point(6, 3)});
    input.emplace_back({gkernel::Point(6, 5), gkernel::Point(9, 6)});
    input.emplace_back({gkernel::Point(6, 4), gkernel::Point(9, 5)});
    input.emplace_back({gkernel::Point(6, 4), gkernel::Point(8, 0)});
    input.emplace_back({gkernel::Point(9, 5), gkernel::Point(11, 7)});
    input.emplace_back({gkernel::Point(8, 0), gkernel::Point(13, 5)});
    input.emplace_back({gkernel::Point(10, 4), gkernel::Point(13, 7)});
    input.emplace_back({gkernel::Point(11, 7), gkernel::Point(13, 5)});
    input.emplace_back({gkernel::Point(10, 4), gkernel::Point(12, 2)});
    input.emplace_back({gkernel::Point(11, 2), gkernel::Point(13, 4)});
    input.emplace_back({gkernel::Point(11, 2), gkernel::Point(12, 1)});
    input.emplace_back({gkernel::Point(12, 1), gkernel::Point(15, 2)});
    input.emplace_back({gkernel::Point(12, 2), gkernel::Point(16, 4)});
    input.emplace_back({gkernel::Point(13, 4), gkernel::Point(15, 2)});
    input.emplace_back({gkernel::Point(13, 7), gkernel::Point(14, 5)});
    input.emplace_back({gkernel::Point(14, 5), gkernel::Point(16, 4)});

    std::vector<gkernel::Point> expected;
    expected.emplace_back(3.5, 5.5);
    expected.emplace_back(4, 3);
    expected.emplace_back(4, 1);
    expected.emplace_back(5, 4);
    expected.emplace_back(5, 2);
    expected.emplace_back(6, 5);
    expected.emplace_back(6, 4);
    expected.emplace_back(6, 3);
    expected.emplace_back(6, 1);
    expected.emplace_back(7, 2);
    expected.emplace_back(8, 0);
    expected.emplace_back(9, 6);
    expected.emplace_back(9, 5);
    expected.emplace_back(9.5, 5.5);
    expected.emplace_back(10, 5);
    expected.emplace_back(10, 4);
    expected.emplace_back(11, 7);
    expected.emplace_back(11, 3);
    expected.emplace_back(11, 2);
    expected.emplace_back(11.5, 2.5);
    expected.emplace_back(12, 6);
    expected.emplace_back(12, 2);
    expected.emplace_back(12, 1);
    expected.emplace_back(13, 7);
    expected.emplace_back(13, 5);
    expected.emplace_back(13, 4);
    expected.emplace_back(14, 5);
    expected.emplace_back(14, 3);
    expected.emplace_back(15, 2);
    expected.emplace_back(16, 4);

    run_intersect_segments_test(input, expected);
}

void TestSegmentsSetIntersectionSecond() {
    gkernel::SegmentsSet input;
    input.emplace_back({gkernel::Point(2, 2), gkernel::Point(9, 9)});
    input.emplace_back({gkernel::Point(3, 9), gkernel::Point(9, 3)});
    input.emplace_back({gkernel::Point(2, 5), gkernel::Point(6, 6)});
    input.emplace_back({gkernel::Point(6, 10), gkernel::Point(14, 2)});
    input.emplace_back({gkernel::Point(6.5, 2), gkernel::Point(8, 4)});
    input.emplace_back({gkernel::Point(6.5, 2), gkernel::Point(9, 3)});
    input.emplace_back({gkernel::Point(2, 5), gkernel::Point(3, 9)});
    input.emplace_back({gkernel::Point(2, 2), gkernel::Point(10, 1)});
    input.emplace_back({gkernel::Point(10, 1), gkernel::Point(14, 2)});
    input.emplace_back({gkernel::Point(6, 10), gkernel::Point(9, 9)});
    input.emplace_back({gkernel::Point(10, 1), gkernel::Point(16, 5)});
    input.emplace_back({gkernel::Point(11, 10), gkernel::Point(19, 2)});
    input.emplace_back({gkernel::Point(13, 6), gkernel::Point(17, 10)});
    input.emplace_back({gkernel::Point(16, 7), gkernel::Point(18, 3)});
    input.emplace_back({gkernel::Point(11, 10), gkernel::Point(21, 8)});
    input.emplace_back({gkernel::Point(16, 7), gkernel::Point(21, 8)});
    input.emplace_back({gkernel::Point(17, 2), gkernel::Point(22, 7)});
    input.emplace_back({gkernel::Point(21, 8), gkernel::Point(22, 7)});
    input.emplace_back({gkernel::Point(2, 5), gkernel::Point(4, 3)});
    input.emplace_back({gkernel::Point(4, 3), gkernel::Point(6.5, 2)});

    std::vector<gkernel::Point> expected;
    expected.emplace_back(2, 2);
    expected.emplace_back(2, 5);
    expected.emplace_back(3, 9);
    expected.emplace_back(3.5, 3.5);
    expected.emplace_back(4, 3);
    expected.emplace_back(6, 6);
    expected.emplace_back(6, 10);
    expected.emplace_back(6.5, 2);
    expected.emplace_back(8, 8);
    expected.emplace_back(8, 4);
    expected.emplace_back(9, 9);
    expected.emplace_back(9, 3);
    expected.emplace_back(10, 1);
    expected.emplace_back(11, 10);
    expected.emplace_back(13, 3);
    expected.emplace_back(14, 7);
    expected.emplace_back(14, 2);
    expected.emplace_back(16, 9);
    expected.emplace_back(16, 7);
    expected.emplace_back(16, 5);
    expected.emplace_back(18, 3);
    expected.emplace_back(21, 8);
    expected.emplace_back(22, 7);

    run_intersect_segments_test(input, expected);
}

void TestSegmentsSetIntersectionThird() {
    gkernel::SegmentsSet input;
    input.emplace_back({gkernel::Point(2, 1), gkernel::Point(6, 5)});
    input.emplace_back({gkernel::Point(6, 5), gkernel::Point(7, 5)});
    input.emplace_back({gkernel::Point(7, 5), gkernel::Point(8, 1)});
    input.emplace_back({gkernel::Point(2, 1), gkernel::Point(8, 1)});
    input.emplace_back({gkernel::Point(2, 7), gkernel::Point(9, 0)});
    input.emplace_back({gkernel::Point(2, 7), gkernel::Point(8, 7)});
    input.emplace_back({gkernel::Point(8, 7), gkernel::Point(9, 0)});
    input.emplace_back({gkernel::Point(9, 6), gkernel::Point(10, 8)});
    input.emplace_back({gkernel::Point(10, 8), gkernel::Point(12, 7)});
    input.emplace_back({gkernel::Point(12, 7), gkernel::Point(13, 5)});
    input.emplace_back({gkernel::Point(13, 5), gkernel::Point(15, 3)});
    input.emplace_back({gkernel::Point(12, 3), gkernel::Point(15, 3)});
    input.emplace_back({gkernel::Point(9, 6), gkernel::Point(12, 3)});
    input.emplace_back({gkernel::Point(12, 5), gkernel::Point(18, 5)});
    input.emplace_back({gkernel::Point(10, 8), gkernel::Point(18, 8)});
    input.emplace_back({gkernel::Point(13, 10), gkernel::Point(18, 5)});
    input.emplace_back({gkernel::Point(11, 9), gkernel::Point(21, 9)});
    input.emplace_back({gkernel::Point(11, 9), gkernel::Point(13, 10)});
    input.emplace_back({gkernel::Point(18, 8), gkernel::Point(21, 9)});
    input.emplace_back({gkernel::Point(10, 8), gkernel::Point(12, 5)});

    std::vector<gkernel::Point> expected;
    expected.emplace_back(2, 7);
    expected.emplace_back(2, 1);
    expected.emplace_back(5, 4);
    expected.emplace_back(6, 5);
    expected.emplace_back(7, 5);
    expected.emplace_back(8, 1);
    expected.emplace_back(8, 7);
    expected.emplace_back(9, 0);
    expected.emplace_back(9, 6);
    expected.emplace_back(10, 8);
    expected.emplace_back(11, 9);
    expected.emplace_back(12, 7);
    expected.emplace_back(12, 5);
    expected.emplace_back(12, 3);
    expected.emplace_back(13, 10);
    expected.emplace_back(13, 5);
    expected.emplace_back(14, 9);
    expected.emplace_back(15, 8);
    expected.emplace_back(15, 3);
    expected.emplace_back(18, 8);
    expected.emplace_back(18, 5);
    expected.emplace_back(21, 9);

    run_intersect_segments_test(input, expected);
}

void TestSegmentsSetIntersectionFifth() {
    gkernel::SegmentsSet input;
    input.emplace_back({gkernel::Point(1, 6), gkernel::Point(1, 2)});
    input.emplace_back({gkernel::Point(1, 6), gkernel::Point(10, 6)});
    input.emplace_back({gkernel::Point(1, 2), gkernel::Point(8, 2)});
    input.emplace_back({gkernel::Point(2, 8), gkernel::Point(2, 1)});
    input.emplace_back({gkernel::Point(6, 8), gkernel::Point(6, 1)});
    input.emplace_back({gkernel::Point(2, 8), gkernel::Point(6, 8)});
    input.emplace_back({gkernel::Point(2, 1), gkernel::Point(6, 1)});
    input.emplace_back({gkernel::Point(8, 9), gkernel::Point(8, 2)});
    input.emplace_back({gkernel::Point(6, 8), gkernel::Point(8, 9)});
    input.emplace_back({gkernel::Point(10, 4), gkernel::Point(16, 4)});
    input.emplace_back({gkernel::Point(10, 6), gkernel::Point(10, 4)});
    input.emplace_back({gkernel::Point(15, 8), gkernel::Point(15, 2)});
    input.emplace_back({gkernel::Point(8, 2), gkernel::Point(15, 2)});
    input.emplace_back({gkernel::Point(8, 9), gkernel::Point(15, 8)});
    input.emplace_back({gkernel::Point(16, 7), gkernel::Point(16, 4)});
    input.emplace_back({gkernel::Point(13, 7), gkernel::Point(16, 7)});
    input.emplace_back({gkernel::Point(13, 7), gkernel::Point(13, 5)});
    input.emplace_back({gkernel::Point(13, 5), gkernel::Point(15, 5)});
    input.emplace_back({gkernel::Point(2, 6), gkernel::Point(6, 2)});
    input.emplace_back({gkernel::Point(2, 2), gkernel::Point(6, 6)});

    std::vector<gkernel::Point> expected;
    expected.emplace_back(1, 6);
    expected.emplace_back(1, 2);
    expected.emplace_back(2, 8);
    expected.emplace_back(2, 6);
    expected.emplace_back(2, 2);
    expected.emplace_back(2, 1);
    expected.emplace_back(4, 4);
    expected.emplace_back(6, 8);
    expected.emplace_back(6, 6);
    expected.emplace_back(6, 2);
    expected.emplace_back(6, 1);
    expected.emplace_back(8, 9);
    expected.emplace_back(8, 6);
    expected.emplace_back(8, 2);
    expected.emplace_back(10, 6);
    expected.emplace_back(10, 4);
    expected.emplace_back(13, 7);
    expected.emplace_back(13, 5);
    expected.emplace_back(15, 8);
    expected.emplace_back(15, 7);
    expected.emplace_back(15, 5);
    expected.emplace_back(15, 4);
    expected.emplace_back(15, 2);
    expected.emplace_back(16, 7);
    expected.emplace_back(16, 4);

    run_intersect_segments_test(input, expected);
}

DECLARE_TEST(TestSegmentsSetIntersectionFirst);
DECLARE_TEST(TestSegmentsSetIntersectionSecond);
DECLARE_TEST(TestSegmentsSetIntersectionThird);
DECLARE_TEST(TestSegmentsSetIntersectionFifth);
