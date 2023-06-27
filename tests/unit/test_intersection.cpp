#include "test.hpp"

#include "gkernel/intersection.hpp"
#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"
#include "gkernel/serializer.hpp"

#include <unordered_set>
#include <algorithm>
#include <iterator>

using namespace gkernel;

void run_intersect_segments_test(gkernel::SegmentsSet& input, std::vector<gkernel::Point>& expected) {
    gkernel::OutputSerializer::serializeSegmentsSet(input, "result.txt");
    auto result = Intersection::intersectSetSegments(input);

    auto points_comparator = [](const gkernel::Point& first, const gkernel::Point& second) {
        if (first.x() != second.x()) {
            return first.x() < second.x();
        } else {
            return first.y() < second.y();
        }
    };

    std::vector<gkernel::Point> result_points;

    for (auto& point : result) {
        result_points.emplace_back(point.point());
    }

    std::sort(result_points.begin(), result_points.end(), points_comparator);
    auto to_erase = std::unique(result_points.begin(), result_points.end(), [](const gkernel::Point& first, const gkernel::Point& second) {
        return first.x() == second.x() && first.y() == second.y();
    });
    result_points.erase(to_erase, result_points.end());

    for (std::size_t idx = 0; idx < result_points.size(); ++idx) {
        std::cout << "result_points[" << idx << "]: " << result_points[idx] << std::endl;
    }

    std::sort(expected.begin(), expected.end(), points_comparator);

    std::set_symmetric_difference(expected.begin(), expected.end(), result_points.begin(), result_points.end(), std::ostream_iterator<gkernel::Point>(std::cout, " "));

    std::cout << std::endl;
    std::cout << "expected_size: " << expected.size() << std::endl;
    std::cout << "result_size: " << result_points.size() << std::endl;


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
    expected.emplace_back(5, 4);
    expected.emplace_back(5, 2);
    expected.emplace_back(7, 2);
    expected.emplace_back(9.5, 5.5);
    expected.emplace_back(11, 3);
    expected.emplace_back(11.5, 2.5);
    expected.emplace_back(12, 6);
    expected.emplace_back(14, 3);

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
    expected.emplace_back(3.5, 3.5);
    expected.emplace_back(6, 6);
    expected.emplace_back(8, 8);
    expected.emplace_back(8, 4);
    expected.emplace_back(13, 3);
    expected.emplace_back(14, 7);
    expected.emplace_back(16, 9);
    expected.emplace_back(16, 5);
    expected.emplace_back(18, 3);

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
    expected.emplace_back(5, 4);
    expected.emplace_back(8, 1);
    expected.emplace_back(13, 5);
    expected.emplace_back(14, 9);
    expected.emplace_back(15, 8);

    run_intersect_segments_test(input, expected);
}

void TestSegmentsSetIntersectionFour() {
    gkernel::SegmentsSet input;
    input.emplace_back({gkernel::Point(3, 12), gkernel::Point(6, 2)});
    input.emplace_back({gkernel::Point(3, 12), gkernel::Point(10, 5)});
    input.emplace_back({gkernel::Point(6, 2), gkernel::Point(10, 4)});
    input.emplace_back({gkernel::Point(6, 12), gkernel::Point(6, 2)});
    input.emplace_back({gkernel::Point(6, 12), gkernel::Point(14, 4)});
    input.emplace_back({gkernel::Point(10, 11), gkernel::Point(10, 0)});
    input.emplace_back({gkernel::Point(10, 11), gkernel::Point(16, 8)});
    input.emplace_back({gkernel::Point(10, 5), gkernel::Point(16, 8)});
    input.emplace_back({gkernel::Point(10, 0), gkernel::Point(14, 4)});
    input.emplace_back({gkernel::Point(14, 12), gkernel::Point(14, 2)});
    input.emplace_back({gkernel::Point(14, 12), gkernel::Point(22, 6)});
    input.emplace_back({gkernel::Point(16, 8), gkernel::Point(20, 0)});
    input.emplace_back({gkernel::Point(14, 2), gkernel::Point(22, 6)});
    input.emplace_back({gkernel::Point(22, 6), gkernel::Point(24, 10)});
    input.emplace_back({gkernel::Point(22, 6), gkernel::Point(26, 2)});
    input.emplace_back({gkernel::Point(20, 0), gkernel::Point(28, 8)});
    input.emplace_back({gkernel::Point(24, 10), gkernel::Point(24, 0)});
    input.emplace_back({gkernel::Point(24, 10), gkernel::Point(26, 6)});
    input.emplace_back({gkernel::Point(24, 0), gkernel::Point(26, 2)});
    input.emplace_back({gkernel::Point(26, 2), gkernel::Point(28, 8)});

    std::vector<gkernel::Point> expected;
    expected.emplace_back(6, 9);
    expected.emplace_back(10, 5);
    expected.emplace_back(10, 4);
    expected.emplace_back(10, 8);

    expected.emplace_back(12, 6);
    expected.emplace_back(14, 9);
    expected.emplace_back(14, 7);
    expected.emplace_back(14, 4);

    expected.emplace_back(18, 4);

    expected.emplace_back(24, 4);
    expected.emplace_back(26, 6);

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
    expected.emplace_back(2, 6);
    expected.emplace_back(2, 2);
    expected.emplace_back(4, 4);
    expected.emplace_back(6, 6);
    expected.emplace_back(6, 2);
    expected.emplace_back(8, 6);
    expected.emplace_back(15, 7);
    expected.emplace_back(15, 5);
    expected.emplace_back(15, 4);

    run_intersect_segments_test(input, expected);
}

void TestSegmentsSetIntersectionSix() {
    gkernel::SegmentsSet input;
    input.emplace_back({gkernel::Point(1, 6), gkernel::Point(1, 2)});
    input.emplace_back({gkernel::Point(2, 10), gkernel::Point(2, 2)});
    input.emplace_back({gkernel::Point(3, 8), gkernel::Point(3, 4)});
    input.emplace_back({gkernel::Point(5, 11), gkernel::Point(5, 5)});
    input.emplace_back({gkernel::Point(8, 8), gkernel::Point(8, 5)});
    input.emplace_back({gkernel::Point(8, 5), gkernel::Point(8, 3)});
    input.emplace_back({gkernel::Point(9, 10), gkernel::Point(9, 4)});
    input.emplace_back({gkernel::Point(11, 5), gkernel::Point(11, 3)});
    input.emplace_back({gkernel::Point(12, 11), gkernel::Point(12, 6)});
    input.emplace_back({gkernel::Point(1, 2), gkernel::Point(2, 2)});
    input.emplace_back({gkernel::Point(8, 3), gkernel::Point(11, 3)});
    input.emplace_back({gkernel::Point(3, 4), gkernel::Point(9, 4)});
    input.emplace_back({gkernel::Point(5, 5), gkernel::Point(8, 5)});
    input.emplace_back({gkernel::Point(8, 5), gkernel::Point(11, 5)});
    input.emplace_back({gkernel::Point(1, 6), gkernel::Point(12, 6)});
    input.emplace_back({gkernel::Point(3, 8), gkernel::Point(8, 8)});
    input.emplace_back({gkernel::Point(2, 10), gkernel::Point(9, 10)});
    input.emplace_back({gkernel::Point(5, 11), gkernel::Point(12, 11)});

    std::vector<gkernel::Point> expected;

    expected.emplace_back(2, 6);
    expected.emplace_back(3, 6);
    expected.emplace_back(5, 10);
    expected.emplace_back(5, 8);
    expected.emplace_back(5, 6);
    expected.emplace_back(8, 6);
    expected.emplace_back(8, 4);
    expected.emplace_back(9, 6);
    expected.emplace_back(9, 5);

    run_intersect_segments_test(input, expected);
}

DECLARE_TEST(TestSegmentsSetIntersectionFirst);
DECLARE_TEST(TestSegmentsSetIntersectionSecond);
DECLARE_TEST(TestSegmentsSetIntersectionThird);
DECLARE_TEST(TestSegmentsSetIntersectionFour);
DECLARE_TEST(TestSegmentsSetIntersectionFifth);
DECLARE_TEST(TestSegmentsSetIntersectionSix);
