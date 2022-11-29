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

void run_intersect_segments_test(gkernel::SegmentsSet& segvec, std::vector<gkernel::Segment>& expected) {
    std::sort(expected.begin(), expected.end(), [](const gkernel::Segment& a, const gkernel::Segment& b) {
        if (a.start().x() != b.start().x()) {
            return a.start().x() < b.start().x();
        } else {
            return a.start().y() < b.start().y();
        }
    });

    // std::unordered_set<gkernel::Segment, SegmentHasher<gkernel::Segment>> points;
    std::vector<gkernel::Segment> points;
    intersectSetSegments(segvec, [&points](const Segment& first, const Segment& second, const Segment& intersection) {
        points.push_back(intersection);
        return true;
        // auto result = points.insert(intersection);
        // return result.second;
    });

    std::sort(points.begin(), points.end(), [](const gkernel::Segment& a, const gkernel::Segment& b) {
        if (a.start().x() != b.start().x()) {
            return a.start().x() < b.start().x();
        } else {
            return a.start().y() < b.start().y();
        }
    });

    std::sort(expected.begin(), expected.end(), [](const gkernel::Segment& a, const gkernel::Segment& b) {
        if (a.start().x() != b.start().x()) {
            return a.start().x() < b.start().x();
        } else {
            return a.start().y() < b.start().y();
        }
    });

    auto last = std::unique(points.begin(), points.end(), [&points](const gkernel::Segment& a, const gkernel::Segment& b) {
        bool result = std::abs(a.start().x() - b.start().x()) < 1e-6 && std::abs(a.start().y() - b.start().y()) < 1e-6;
        return result;
    });

    points.erase(last, points.end());
    std::cout << "result size: " << points.size() << std::endl;
    // print points in format x, y
    for (auto& point : points)
    {
        std::cout << point.start().x() << ", " << point.start().y() << std::endl;
    }

    REQUIRE_EQ(std::equal(points.begin(), points.end(), expected.begin(), expected.end(), [](const gkernel::Segment& a, const gkernel::Segment& b) {
        return std::abs(a.start().x() - b.start().x()) < 1e-6 && std::abs(a.start().y() - b.start().y()) < 1e-6;
    }), true);
}

void Test1() {
    gkernel::SegmentsSet segvec;
    segvec.emplace_back({gkernel::Point(9, 6), gkernel::Point(10, 5)});
    segvec.emplace_back({gkernel::Point(6, 1), gkernel::Point(10, 5)});
    segvec.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(4, 1)});
    segvec.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(6, 3)});
    segvec.emplace_back({gkernel::Point(4, 3), gkernel::Point(6, 1)});
    segvec.emplace_back({gkernel::Point(4, 3), gkernel::Point(6, 5)});
    segvec.emplace_back({gkernel::Point(4, 1), gkernel::Point(6, 3)});
    segvec.emplace_back({gkernel::Point(6, 5), gkernel::Point(9, 6)});
    segvec.emplace_back({gkernel::Point(6, 4), gkernel::Point(9, 5)});
    segvec.emplace_back({gkernel::Point(6, 4), gkernel::Point(8, 0)});
    segvec.emplace_back({gkernel::Point(9, 5), gkernel::Point(11, 7)});
    segvec.emplace_back({gkernel::Point(8, 0), gkernel::Point(13, 5)});
    segvec.emplace_back({gkernel::Point(10, 4), gkernel::Point(13, 7)});
    segvec.emplace_back({gkernel::Point(11, 7), gkernel::Point(13, 5)});
    segvec.emplace_back({gkernel::Point(10, 4), gkernel::Point(12, 2)});
    segvec.emplace_back({gkernel::Point(11, 2), gkernel::Point(13, 4)});
    segvec.emplace_back({gkernel::Point(11, 2), gkernel::Point(12, 1)});
    segvec.emplace_back({gkernel::Point(12, 1), gkernel::Point(15, 2)});
    segvec.emplace_back({gkernel::Point(12, 2), gkernel::Point(16, 4)});
    segvec.emplace_back({gkernel::Point(13, 4), gkernel::Point(15, 2)});
    segvec.emplace_back({gkernel::Point(13, 7), gkernel::Point(14, 5)});
    segvec.emplace_back({gkernel::Point(14, 5), gkernel::Point(16, 4)});

    std::vector<gkernel::Segment> expected;
    expected.push_back({gkernel::Point(3.5, 5.5), gkernel::Point(3.5, 5.5)});
    expected.push_back({gkernel::Point(4, 3), gkernel::Point(4, 3)});
    expected.push_back({gkernel::Point(4, 1), gkernel::Point(4, 1)});
    expected.push_back({gkernel::Point(5, 4), gkernel::Point(5, 4)});
    expected.push_back({gkernel::Point(5, 2), gkernel::Point(5, 2)});
    expected.push_back({gkernel::Point(6, 5), gkernel::Point(6, 5)});
    expected.push_back({gkernel::Point(6, 4), gkernel::Point(6, 4)});
    expected.push_back({gkernel::Point(6, 3), gkernel::Point(6, 3)});
    expected.push_back({gkernel::Point(6, 1), gkernel::Point(6, 1)});
    expected.push_back({gkernel::Point(7, 2), gkernel::Point(7, 2)});
    expected.push_back({gkernel::Point(8, 0), gkernel::Point(8, 0)});
    expected.push_back({gkernel::Point(9, 6), gkernel::Point(9, 6)});
    expected.push_back({gkernel::Point(9, 5), gkernel::Point(9, 5)});
    expected.push_back({gkernel::Point(9.5, 5.5), gkernel::Point(9.5, 5.5)});
    expected.push_back({gkernel::Point(10, 5), gkernel::Point(10, 5)});
    expected.push_back({gkernel::Point(10, 4), gkernel::Point(10, 4)});
    expected.push_back({gkernel::Point(11, 7), gkernel::Point(11, 7)});
    expected.push_back({gkernel::Point(11, 3), gkernel::Point(11, 3)});
    expected.push_back({gkernel::Point(11, 2), gkernel::Point(11, 2)});
    expected.push_back({gkernel::Point(11.5, 2.5), gkernel::Point(11.5, 2.5)});
    expected.push_back({gkernel::Point(12, 6), gkernel::Point(12, 6)});
    expected.push_back({gkernel::Point(12, 2), gkernel::Point(12, 2)});
    expected.push_back({gkernel::Point(12, 1), gkernel::Point(12, 1)});
    expected.push_back({gkernel::Point(13, 7), gkernel::Point(13, 7)});
    expected.push_back({gkernel::Point(13, 5), gkernel::Point(13, 5)});
    expected.push_back({gkernel::Point(13, 4), gkernel::Point(13, 4)});
    expected.push_back({gkernel::Point(14, 5), gkernel::Point(14, 5)});
    expected.push_back({gkernel::Point(14, 3), gkernel::Point(14, 3)});
    expected.push_back({gkernel::Point(15, 2), gkernel::Point(15, 2)});
    expected.push_back({gkernel::Point(16, 4), gkernel::Point(16, 4)});
    run_intersect_segments_test(segvec, expected);
}

void Test2() {
    gkernel::SegmentsSet segvec;
    segvec.emplace_back({gkernel::Point(2, 2), gkernel::Point(9, 9)});
    segvec.emplace_back({gkernel::Point(3, 9), gkernel::Point(9, 3)});
    segvec.emplace_back({gkernel::Point(2, 5), gkernel::Point(6, 6)});
    segvec.emplace_back({gkernel::Point(6, 10), gkernel::Point(14, 2)});
    segvec.emplace_back({gkernel::Point(6.5, 2), gkernel::Point(8, 4)});
    segvec.emplace_back({gkernel::Point(6.5, 2), gkernel::Point(9, 3)});
    segvec.emplace_back({gkernel::Point(2, 5), gkernel::Point(3, 9)});
    segvec.emplace_back({gkernel::Point(2, 2), gkernel::Point(10, 1)});
    segvec.emplace_back({gkernel::Point(10, 1), gkernel::Point(14, 2)});
    segvec.emplace_back({gkernel::Point(6, 10), gkernel::Point(9, 9)});
    segvec.emplace_back({gkernel::Point(10, 1), gkernel::Point(16, 5)});
    segvec.emplace_back({gkernel::Point(11, 10), gkernel::Point(19, 2)});
    segvec.emplace_back({gkernel::Point(13, 6), gkernel::Point(17, 10)});
    segvec.emplace_back({gkernel::Point(16, 7), gkernel::Point(18, 3)});
    segvec.emplace_back({gkernel::Point(11, 10), gkernel::Point(21, 8)});
    segvec.emplace_back({gkernel::Point(16, 7), gkernel::Point(21, 8)});
    segvec.emplace_back({gkernel::Point(17, 2), gkernel::Point(22, 7)});
    segvec.emplace_back({gkernel::Point(21, 8), gkernel::Point(22, 7)});
    segvec.emplace_back({gkernel::Point(2, 5), gkernel::Point(4, 3)});
    segvec.emplace_back({gkernel::Point(4, 3), gkernel::Point(6.5, 2)});

    std::vector<gkernel::Segment> expected;
    expected.push_back({gkernel::Point(2, 2), gkernel::Point(2, 2)});
    expected.push_back({gkernel::Point(2, 5), gkernel::Point(2, 5)});
    expected.push_back({gkernel::Point(3, 9), gkernel::Point(3, 9)});
    expected.push_back({gkernel::Point(3.5, 3.5), gkernel::Point(3.5, 3.5)});
    expected.push_back({gkernel::Point(4, 3), gkernel::Point(4, 3)});
    expected.push_back({gkernel::Point(6, 6), gkernel::Point(6, 6)});
    expected.push_back({gkernel::Point(6, 10), gkernel::Point(6, 10)});
    expected.push_back({gkernel::Point(6.5, 2), gkernel::Point(6.5, 2)});
    expected.push_back({gkernel::Point(8, 8), gkernel::Point(8, 8)});
    expected.push_back({gkernel::Point(8, 4), gkernel::Point(8, 4)});
    expected.push_back({gkernel::Point(9, 9), gkernel::Point(9, 9)});
    expected.push_back({gkernel::Point(9, 3), gkernel::Point(9, 3)});
    expected.push_back({gkernel::Point(10, 1), gkernel::Point(10, 1)});
    expected.push_back({gkernel::Point(11, 10), gkernel::Point(11, 10)});
    expected.push_back({gkernel::Point(13, 3), gkernel::Point(13, 3)});
    expected.push_back({gkernel::Point(14, 7), gkernel::Point(14, 7)});
    expected.push_back({gkernel::Point(14, 2), gkernel::Point(14, 2)});
    expected.push_back({gkernel::Point(16, 9), gkernel::Point(16, 9)});
    expected.push_back({gkernel::Point(16, 7), gkernel::Point(16, 7)});
    expected.push_back({gkernel::Point(16, 5), gkernel::Point(16, 5)});
    expected.push_back({gkernel::Point(18, 3), gkernel::Point(18, 3)});
    expected.push_back({gkernel::Point(21, 8), gkernel::Point(21, 8)});
    expected.push_back({gkernel::Point(22, 7), gkernel::Point(22, 7)});
    run_intersect_segments_test(segvec, expected);
}

void Test3() {
    gkernel::SegmentsSet segvec;
    segvec.emplace_back({gkernel::Point(2, 1), gkernel::Point(6, 5)});
    segvec.emplace_back({gkernel::Point(6, 5), gkernel::Point(7, 5)});
    segvec.emplace_back({gkernel::Point(7, 5), gkernel::Point(8, 1)});
    segvec.emplace_back({gkernel::Point(2, 1), gkernel::Point(8, 1)});
    segvec.emplace_back({gkernel::Point(2, 7), gkernel::Point(9, 0)});
    segvec.emplace_back({gkernel::Point(2, 7), gkernel::Point(8, 7)});
    segvec.emplace_back({gkernel::Point(8, 7), gkernel::Point(9, 0)});
    segvec.emplace_back({gkernel::Point(9, 6), gkernel::Point(10, 8)});
    segvec.emplace_back({gkernel::Point(10, 8), gkernel::Point(12, 7)});
    segvec.emplace_back({gkernel::Point(12, 7), gkernel::Point(13, 5)});
    segvec.emplace_back({gkernel::Point(13, 5), gkernel::Point(15, 3)});
    segvec.emplace_back({gkernel::Point(12, 3), gkernel::Point(15, 3)});
    segvec.emplace_back({gkernel::Point(9, 6), gkernel::Point(12, 3)});
    segvec.emplace_back({gkernel::Point(12, 5), gkernel::Point(18, 5)});
    segvec.emplace_back({gkernel::Point(10, 8), gkernel::Point(18, 8)});
    segvec.emplace_back({gkernel::Point(13, 10), gkernel::Point(18, 5)});
    segvec.emplace_back({gkernel::Point(11, 9), gkernel::Point(21, 9)});
    segvec.emplace_back({gkernel::Point(11, 9), gkernel::Point(13, 10)});
    segvec.emplace_back({gkernel::Point(18, 8), gkernel::Point(21, 9)});
    segvec.emplace_back({gkernel::Point(10, 8), gkernel::Point(12, 5)});

    std::vector<gkernel::Segment> expected;
    expected.push_back({gkernel::Point(2, 7), gkernel::Point(2, 7)});
    expected.push_back({gkernel::Point(2, 1), gkernel::Point(2, 1)});
    expected.push_back({gkernel::Point(5, 4), gkernel::Point(5, 4)});
    expected.push_back({gkernel::Point(6, 5), gkernel::Point(6, 5)});
    expected.push_back({gkernel::Point(7, 5), gkernel::Point(7, 5)});
    expected.push_back({gkernel::Point(8, 1), gkernel::Point(8, 1)});
    expected.push_back({gkernel::Point(8, 7), gkernel::Point(8, 7)});
    expected.push_back({gkernel::Point(9, 0), gkernel::Point(9, 0)});
    expected.push_back({gkernel::Point(9, 6), gkernel::Point(9, 6)});
    expected.push_back({gkernel::Point(10, 8), gkernel::Point(10, 8)});
    expected.push_back({gkernel::Point(11, 9), gkernel::Point(11, 9)});
    expected.push_back({gkernel::Point(12, 7), gkernel::Point(12, 7)});
    expected.push_back({gkernel::Point(12, 5), gkernel::Point(12, 5)});
    expected.push_back({gkernel::Point(12, 3), gkernel::Point(12, 3)});
    expected.push_back({gkernel::Point(13, 10), gkernel::Point(13, 10)});
    expected.push_back({gkernel::Point(13, 5), gkernel::Point(13, 5)});
    expected.push_back({gkernel::Point(14, 9), gkernel::Point(14, 9)});
    expected.push_back({gkernel::Point(15, 8), gkernel::Point(15, 8)});
    expected.push_back({gkernel::Point(15, 3), gkernel::Point(15, 3)});
    expected.push_back({gkernel::Point(18, 8), gkernel::Point(18, 8)});
    expected.push_back({gkernel::Point(18, 5), gkernel::Point(18, 5)});
    expected.push_back({gkernel::Point(21, 9), gkernel::Point(21, 9)});

    run_intersect_segments_test(segvec, expected);
}

void Test4()
{
    gkernel::SegmentsSet segvec;
    segvec.emplace_back({gkernel::Point(3, 12), gkernel::Point(6, 2)});
    segvec.emplace_back({gkernel::Point(3, 12), gkernel::Point(10, 5)});
    segvec.emplace_back({gkernel::Point(6, 2), gkernel::Point(10, 4)});
    segvec.emplace_back({gkernel::Point(6, 12), gkernel::Point(6, 2)});
    segvec.emplace_back({gkernel::Point(6, 12), gkernel::Point(14, 4)});
    segvec.emplace_back({gkernel::Point(10, 11), gkernel::Point(10, 0)});
    segvec.emplace_back({gkernel::Point(10, 11), gkernel::Point(16, 8)});
    segvec.emplace_back({gkernel::Point(10, 5), gkernel::Point(16, 8)});
    segvec.emplace_back({gkernel::Point(10, 0), gkernel::Point(14, 4)});
    segvec.emplace_back({gkernel::Point(14, 12), gkernel::Point(14, 2)});
    segvec.emplace_back({gkernel::Point(14, 12), gkernel::Point(22, 6)});
    segvec.emplace_back({gkernel::Point(16, 8), gkernel::Point(20, 0)});
    segvec.emplace_back({gkernel::Point(14, 2), gkernel::Point(22, 6)});
    segvec.emplace_back({gkernel::Point(22, 6), gkernel::Point(24, 10)});
    segvec.emplace_back({gkernel::Point(22, 6), gkernel::Point(26, 2)});
    segvec.emplace_back({gkernel::Point(20, 0), gkernel::Point(28, 8)});
    segvec.emplace_back({gkernel::Point(24, 10), gkernel::Point(24, 0)});
    segvec.emplace_back({gkernel::Point(24, 10), gkernel::Point(26, 6)});
    segvec.emplace_back({gkernel::Point(24, 0), gkernel::Point(26, 2)});
    segvec.emplace_back({gkernel::Point(26, 2), gkernel::Point(28, 8)});

    std::vector<gkernel::Segment> expected;
    expected.push_back({gkernel::Point(3, 12), gkernel::Point(3, 12)});
    expected.push_back({gkernel::Point(6, 2), gkernel::Point(6, 2)});
    expected.push_back({gkernel::Point(6, 9), gkernel::Point(6, 9)});
    expected.push_back({gkernel::Point(6, 12), gkernel::Point(6, 12)});
    expected.push_back({gkernel::Point(10, 11), gkernel::Point(10, 11)});
    expected.push_back({gkernel::Point(10, 5), gkernel::Point(10, 5)});
    expected.push_back({gkernel::Point(10, 4), gkernel::Point(10, 4)});
    expected.push_back({gkernel::Point(10, 0), gkernel::Point(10, 0)});
    expected.push_back({gkernel::Point(10, 8), gkernel::Point(10, 8)});
    expected.push_back({gkernel::Point(12, 6), gkernel::Point(12, 6)});
    expected.push_back({gkernel::Point(14, 12), gkernel::Point(14, 12)});
    expected.push_back({gkernel::Point(14, 9), gkernel::Point(14, 9)});
    expected.push_back({gkernel::Point(14, 7), gkernel::Point(14, 7)});
    expected.push_back({gkernel::Point(14, 4), gkernel::Point(14, 4)});
    expected.push_back({gkernel::Point(14, 2), gkernel::Point(14, 2)});
    expected.push_back({gkernel::Point(16, 8), gkernel::Point(16, 8)});
    expected.push_back({gkernel::Point(18, 4), gkernel::Point(18, 4)});
    expected.push_back({gkernel::Point(20, 0), gkernel::Point(20, 0)});
    expected.push_back({gkernel::Point(22, 6), gkernel::Point(22, 6)});
    expected.push_back({gkernel::Point(24, 10), gkernel::Point(24, 10)});
    expected.push_back({gkernel::Point(24, 4), gkernel::Point(24, 4)});
    expected.push_back({gkernel::Point(24, 0), gkernel::Point(24, 0)});
    expected.push_back({gkernel::Point(26, 2), gkernel::Point(26, 2)});
    expected.push_back({gkernel::Point(26, 6), gkernel::Point(26, 6)});
    expected.push_back({gkernel::Point(28, 8), gkernel::Point(28, 8)});

    run_intersect_segments_test(segvec, expected);

}

void Test5()
{
    gkernel::SegmentsSet segvec;
    segvec.emplace_back({gkernel::Point(1, 6), gkernel::Point(1, 2)});
    segvec.emplace_back({gkernel::Point(1, 6), gkernel::Point(10, 6)});
    segvec.emplace_back({gkernel::Point(1, 2), gkernel::Point(8, 2)});
    segvec.emplace_back({gkernel::Point(2, 8), gkernel::Point(2, 1)});
    segvec.emplace_back({gkernel::Point(6, 8), gkernel::Point(6, 1)});
    segvec.emplace_back({gkernel::Point(2, 8), gkernel::Point(6, 8)});
    segvec.emplace_back({gkernel::Point(2, 1), gkernel::Point(6, 1)});
    segvec.emplace_back({gkernel::Point(8, 9), gkernel::Point(8, 2)});
    segvec.emplace_back({gkernel::Point(6, 8), gkernel::Point(8, 9)});
    segvec.emplace_back({gkernel::Point(10, 4), gkernel::Point(16, 4)});
    segvec.emplace_back({gkernel::Point(10, 6), gkernel::Point(10, 4)});
    segvec.emplace_back({gkernel::Point(15, 8), gkernel::Point(15, 2)});
    segvec.emplace_back({gkernel::Point(8, 2), gkernel::Point(15, 2)});
    segvec.emplace_back({gkernel::Point(8, 9), gkernel::Point(15, 8)});
    segvec.emplace_back({gkernel::Point(16, 7), gkernel::Point(16, 4)});
    segvec.emplace_back({gkernel::Point(13, 7), gkernel::Point(16, 7)});
    segvec.emplace_back({gkernel::Point(13, 7), gkernel::Point(13, 5)});
    segvec.emplace_back({gkernel::Point(13, 5), gkernel::Point(15, 5)});
    segvec.emplace_back({gkernel::Point(2, 6), gkernel::Point(6, 2)});
    segvec.emplace_back({gkernel::Point(2, 2), gkernel::Point(6, 6)});

    std::vector<gkernel::Segment> expected;
    expected.push_back({gkernel::Point(1, 6), gkernel::Point(1, 6)});
    expected.push_back({gkernel::Point(1, 2), gkernel::Point(1, 2)});
    expected.push_back({gkernel::Point(2, 8), gkernel::Point(2, 8)});
    expected.push_back({gkernel::Point(2, 6), gkernel::Point(2, 6)});
    expected.push_back({gkernel::Point(2, 2), gkernel::Point(2, 2)});
    expected.push_back({gkernel::Point(2, 1), gkernel::Point(2, 1)});
    expected.push_back({gkernel::Point(4, 4), gkernel::Point(4, 4)});
    expected.push_back({gkernel::Point(6, 8), gkernel::Point(6, 8)});
    expected.push_back({gkernel::Point(6, 6), gkernel::Point(6, 6)});
    expected.push_back({gkernel::Point(6, 2), gkernel::Point(6, 2)});
    expected.push_back({gkernel::Point(6, 1), gkernel::Point(6, 1)});
    expected.push_back({gkernel::Point(8, 9), gkernel::Point(8, 9)});
    expected.push_back({gkernel::Point(8, 6), gkernel::Point(8, 6)});
    expected.push_back({gkernel::Point(8, 2), gkernel::Point(8, 2)});
    expected.push_back({gkernel::Point(10, 6), gkernel::Point(10, 6)});
    expected.push_back({gkernel::Point(10, 4), gkernel::Point(10, 4)});
    expected.push_back({gkernel::Point(13, 7), gkernel::Point(13, 7)});
    expected.push_back({gkernel::Point(13, 5), gkernel::Point(13, 5)});
    expected.push_back({gkernel::Point(15, 8), gkernel::Point(15, 8)});
    expected.push_back({gkernel::Point(15, 7), gkernel::Point(15, 7)});
    expected.push_back({gkernel::Point(15, 5), gkernel::Point(15, 5)});
    expected.push_back({gkernel::Point(15, 4), gkernel::Point(15, 4)});
    expected.push_back({gkernel::Point(15, 2), gkernel::Point(15, 2)});
    expected.push_back({gkernel::Point(16, 7), gkernel::Point(16, 7)});
    expected.push_back({gkernel::Point(16, 4), gkernel::Point(16, 4)});

    run_intersect_segments_test(segvec, expected);

}

void Test6()
{
    gkernel::SegmentsSet segvec;
    segvec.emplace_back({gkernel::Point(1, 6), gkernel::Point(1, 2)});
    segvec.emplace_back({gkernel::Point(2, 10), gkernel::Point(2, 2)});
    segvec.emplace_back({gkernel::Point(3, 8), gkernel::Point(3, 4)});
    segvec.emplace_back({gkernel::Point(5, 11), gkernel::Point(5, 5)});
    segvec.emplace_back({gkernel::Point(8, 8), gkernel::Point(8, 5)});
    segvec.emplace_back({gkernel::Point(8, 5), gkernel::Point(8, 3)});
    segvec.emplace_back({gkernel::Point(9, 10), gkernel::Point(9, 4)});
    segvec.emplace_back({gkernel::Point(11, 5), gkernel::Point(11, 3)});
    segvec.emplace_back({gkernel::Point(12, 11), gkernel::Point(12, 6)});
    segvec.emplace_back({gkernel::Point(1, 2), gkernel::Point(2, 2)});
    segvec.emplace_back({gkernel::Point(8, 3), gkernel::Point(11, 3)});
    segvec.emplace_back({gkernel::Point(3, 4), gkernel::Point(9, 4)});
    segvec.emplace_back({gkernel::Point(5, 5), gkernel::Point(8, 5)});
    segvec.emplace_back({gkernel::Point(8, 5), gkernel::Point(11, 5)});
    segvec.emplace_back({gkernel::Point(1, 6), gkernel::Point(12, 6)});
    segvec.emplace_back({gkernel::Point(3, 8), gkernel::Point(8, 8)});
    segvec.emplace_back({gkernel::Point(2, 10), gkernel::Point(9, 10)});
    segvec.emplace_back({gkernel::Point(5, 11), gkernel::Point(12, 11)});

    std::vector<gkernel::Segment> expected;
    expected.push_back({gkernel::Point(1, 6), gkernel::Point(1, 6)});
    expected.push_back({gkernel::Point(1, 2), gkernel::Point(1, 2)});
    expected.push_back({gkernel::Point(2, 10), gkernel::Point(2, 10)});
    expected.push_back({gkernel::Point(2, 6), gkernel::Point(2, 6)});
    expected.push_back({gkernel::Point(2, 2), gkernel::Point(2, 2)});
    expected.push_back({gkernel::Point(3, 8), gkernel::Point(3, 8)});
    expected.push_back({gkernel::Point(3, 6), gkernel::Point(3, 6)});
    expected.push_back({gkernel::Point(3, 4), gkernel::Point(3, 4)});
    expected.push_back({gkernel::Point(5, 11), gkernel::Point(5, 11)});
    expected.push_back({gkernel::Point(5, 10), gkernel::Point(5, 10)});
    expected.push_back({gkernel::Point(5, 8), gkernel::Point(5, 8)});
    expected.push_back({gkernel::Point(5, 6), gkernel::Point(5, 6)});
    expected.push_back({gkernel::Point(5, 5), gkernel::Point(5, 5)});
    expected.push_back({gkernel::Point(8, 8), gkernel::Point(8, 8)});
    expected.push_back({gkernel::Point(8, 6), gkernel::Point(8, 6)});
    expected.push_back({gkernel::Point(8, 5), gkernel::Point(8, 5)});
    expected.push_back({gkernel::Point(8, 4), gkernel::Point(8, 4)});
    expected.push_back({gkernel::Point(8, 3), gkernel::Point(8, 3)});
    expected.push_back({gkernel::Point(9, 10), gkernel::Point(9, 10)});
    expected.push_back({gkernel::Point(9, 6), gkernel::Point(9, 6)});
    expected.push_back({gkernel::Point(9, 5), gkernel::Point(9, 5)});
    expected.push_back({gkernel::Point(9, 4), gkernel::Point(9, 4)});
    expected.push_back({gkernel::Point(11, 5), gkernel::Point(11, 5)});
    expected.push_back({gkernel::Point(11, 3), gkernel::Point(11, 3)});
    expected.push_back({gkernel::Point(12, 11), gkernel::Point(12, 11)});
    expected.push_back({gkernel::Point(12, 6), gkernel::Point(12, 6)});

    run_intersect_segments_test(segvec, expected);

}
void Test7()
{
    gkernel::SegmentsSet segvec;
    segvec.emplace_back({gkernel::Point(2, 2), gkernel::Point(7, 7)});
    segvec.emplace_back({gkernel::Point(2, 2), gkernel::Point(7, 7)});
    segvec.emplace_back({gkernel::Point(2, 4), gkernel::Point(5, 1)});
    segvec.emplace_back({gkernel::Point(3, 3), gkernel::Point(5, 1)});
    segvec.emplace_back({gkernel::Point(3, 5), gkernel::Point(6, 2)});
    segvec.emplace_back({gkernel::Point(4, 10), gkernel::Point(7, 7)});
    segvec.emplace_back({gkernel::Point(5, 9), gkernel::Point(6, 8)});
    segvec.emplace_back({gkernel::Point(5, 9), gkernel::Point(6, 8)});
    segvec.emplace_back({gkernel::Point(5, 7), gkernel::Point(11, 1)});
    segvec.emplace_back({gkernel::Point(8, 4), gkernel::Point(10, 2)});
    segvec.emplace_back({gkernel::Point(8, 4), gkernel::Point(10, 2)});
    segvec.emplace_back({gkernel::Point(6, 2), gkernel::Point(10, 6)});
    segvec.emplace_back({gkernel::Point(8, 6), gkernel::Point(12, 2)});
    segvec.emplace_back({gkernel::Point(9, 1), gkernel::Point(13, 5)});
    segvec.emplace_back({gkernel::Point(11, 5), gkernel::Point(14, 2)});
    segvec.emplace_back({gkernel::Point(11, 1), gkernel::Point(15, 5)});
    segvec.emplace_back({gkernel::Point(13, 3), gkernel::Point(15, 5)});
    segvec.emplace_back({gkernel::Point(13, 3), gkernel::Point(15, 5)});

    std::vector<gkernel::Segment> expected;
    expected.push_back({gkernel::Point(3, 3), gkernel::Point(3, 3)});
    expected.push_back({gkernel::Point(3, 3), gkernel::Point(5, 1)});
    expected.push_back({gkernel::Point(2, 2), gkernel::Point(7, 7)});
    expected.push_back({gkernel::Point(5, 9), gkernel::Point(6, 8)});
    expected.push_back({gkernel::Point(4, 4), gkernel::Point(4, 4)});
    expected.push_back({gkernel::Point(6, 6), gkernel::Point(6, 6)});
    expected.push_back({gkernel::Point(6, 2), gkernel::Point(6, 2)});
    expected.push_back({gkernel::Point(7, 7), gkernel::Point(7, 7)});
    expected.push_back({gkernel::Point(8, 4), gkernel::Point(10, 2)});
    expected.push_back({gkernel::Point(8, 4), gkernel::Point(8, 4)});
    expected.push_back({gkernel::Point(9, 5), gkernel::Point(9, 5)});
    expected.push_back({gkernel::Point(10, 2), gkernel::Point(10, 2)});
    expected.push_back({gkernel::Point(11, 3), gkernel::Point(11, 3)});
    expected.push_back({gkernel::Point(11, 1), gkernel::Point(11, 1)});
    expected.push_back({gkernel::Point(12, 4), gkernel::Point(12, 4)});
    expected.push_back({gkernel::Point(12, 2), gkernel::Point(12, 2)});
    expected.push_back({gkernel::Point(13, 3), gkernel::Point(13, 3)});
    expected.push_back({gkernel::Point(13, 3), gkernel::Point(15, 5)});

    run_intersect_segments_test(segvec, expected);

}
void Test8()
{
    gkernel::SegmentsSet segvec;
    segvec.emplace_back({gkernel::Point(1, 1), gkernel::Point(6, 6)});
    segvec.emplace_back({gkernel::Point(1, 1), gkernel::Point(6, 6)});
    segvec.emplace_back({gkernel::Point(1, 1), gkernel::Point(6, 6)});
    segvec.emplace_back({gkernel::Point(1, 1), gkernel::Point(6, 1)});
    segvec.emplace_back({gkernel::Point(6, 6), gkernel::Point(6, 1)});
    segvec.emplace_back({gkernel::Point(1, 1), gkernel::Point(6, 6)});
    segvec.emplace_back({gkernel::Point(1, 1), gkernel::Point(6, 1)});
    segvec.emplace_back({gkernel::Point(6, 6), gkernel::Point(6, 1)});
    segvec.emplace_back({gkernel::Point(4, 2), gkernel::Point(4, 7)});
    segvec.emplace_back({gkernel::Point(4, 7), gkernel::Point(11, 7)});
    segvec.emplace_back({gkernel::Point(4, 2), gkernel::Point(11, 2)});
    segvec.emplace_back({gkernel::Point(11, 7), gkernel::Point(11, 2)});
    segvec.emplace_back({gkernel::Point(10, 7), gkernel::Point(11, 7)});
    segvec.emplace_back({gkernel::Point(10, 7), gkernel::Point(11, 7)});
    segvec.emplace_back({gkernel::Point(11, 7), gkernel::Point(11, 6)});
    segvec.emplace_back({gkernel::Point(11, 7), gkernel::Point(11, 6)});
    segvec.emplace_back({gkernel::Point(11, 7), gkernel::Point(11, 6)});
    segvec.emplace_back({gkernel::Point(8, 2), gkernel::Point(10, 2)});
    segvec.emplace_back({gkernel::Point(8, 5), gkernel::Point(8, 2)});
    segvec.emplace_back({gkernel::Point(8, 5), gkernel::Point(10, 5)});
    segvec.emplace_back({gkernel::Point(10, 5), gkernel::Point(10, 2)});

    std::vector<gkernel::Segment> expected;
    expected.push_back({gkernel::Point(1, 1), gkernel::Point(1, 1)});
    expected.push_back({gkernel::Point(4, 7), gkernel::Point(4, 7)});
    expected.push_back({gkernel::Point(4, 4), gkernel::Point(4, 4)});
    expected.push_back({gkernel::Point(4, 2), gkernel::Point(4, 2)});
    expected.push_back({gkernel::Point(6, 6), gkernel::Point(6, 6)});
    expected.push_back({gkernel::Point(6, 2), gkernel::Point(6, 2)});
    expected.push_back({gkernel::Point(6, 1), gkernel::Point(6, 1)});
    expected.push_back({gkernel::Point(8, 5), gkernel::Point(8, 5)});
    expected.push_back({gkernel::Point(10, 5), gkernel::Point(10, 5)});
    expected.push_back({gkernel::Point(11, 7), gkernel::Point(11, 7)});
    expected.push_back({gkernel::Point(11, 2), gkernel::Point(11, 2)});
    segvec.emplace_back({gkernel::Point(1, 1), gkernel::Point(6, 6)});
    segvec.emplace_back({gkernel::Point(1, 1), gkernel::Point(6, 1)});
    segvec.emplace_back({gkernel::Point(6, 6), gkernel::Point(6, 1)});
    segvec.emplace_back({gkernel::Point(8, 2), gkernel::Point(10, 2)});
    segvec.emplace_back({gkernel::Point(10, 7), gkernel::Point(11, 7)});
    segvec.emplace_back({gkernel::Point(11, 7), gkernel::Point(11, 6)});

    run_intersect_segments_test(segvec, expected);

}

void Test9() {
    gkernel::SegmentsSet segvec;
    segvec.emplace_back({gkernel::Point(4, 3), gkernel::Point(8, 8)});
    segvec.emplace_back({gkernel::Point(0, 1), gkernel::Point(2, 8)});
    segvec.emplace_back({gkernel::Point(2, 1), gkernel::Point(5, 5)});
    segvec.emplace_back({gkernel::Point(4, 0), gkernel::Point(8, 6)});
    segvec.emplace_back({gkernel::Point(3, 3), gkernel::Point(4, 4)});
    segvec.emplace_back({gkernel::Point(3, 3), gkernel::Point(8, 5)});
    segvec.emplace_back({gkernel::Point(1, 4), gkernel::Point(5, 10)});
    segvec.emplace_back({gkernel::Point(1, 2), gkernel::Point(6, 8)});
    segvec.emplace_back({gkernel::Point(1, 4), gkernel::Point(2, 10)});
    segvec.emplace_back({gkernel::Point(3, 1), gkernel::Point(7, 2)});
    segvec.emplace_back({gkernel::Point(0, 3), gkernel::Point(1, 6)});
    segvec.emplace_back({gkernel::Point(0, 3), gkernel::Point(2, 6)});
    std::vector<gkernel::Segment> expected;
    run_intersect_segments_test(segvec, expected);
}


TEST_CASE("Test intersection 1") {
    Test1();
}

TEST_CASE("Test intersection 2") {
    Test2();
}

TEST_CASE("Test intersection 3") {
    Test3();
}

// TEST_CASE("Test intersection 4") {
//     Test4();
// }

// TEST_CASE("Test intersection 5") {
//     Test5();
// }

// TEST_CASE("Test intersection 6") {
//     Test6();
// }

// TEST_CASE("Test intersection 7") {
//     Test7();
// }

// TEST_CASE("Test intersection 8") {
//     Test8();
// }

// TEST_CASE("Test intersection 9") {
//     Test9();
// }
