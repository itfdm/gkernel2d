#include "test.hpp"
#include "gkernel/objects.hpp"
#include "gkernel/intersection.hpp"
#include <vector>
#include <set>

void test_no_intersection()
{
    gkernel::Point point_a1(1, 5), point_b1(3, 1);
    gkernel::Point point_a2(3, 3), point_b2(7, 3);

    gkernel::Segment s1(point_a1, point_b1);
    gkernel::Segment s2(point_a2, point_b2);
    gkernel::Segment res = gkernel::pointintersection(s1, s2);
    gkernel::Point result_point;

    REQUIRE_EQ(result_point, res.begin_point());
    REQUIRE_EQ(result_point, res.end_point());
}

void test_inner_intersection()
{
    gkernel::Point point_a1(1, 5), point_b1(3, 1);
    gkernel::Point point_a2(1, 3), point_b2(7, 3);
    gkernel::Point result_point(2, 3);

    gkernel::Segment s1(point_a1, point_b1);
    gkernel::Segment s2(point_a2, point_b2);
    gkernel::Segment res = gkernel::pointintersection(s1, s2);

    REQUIRE_EQ(result_point, res.begin_point());
    REQUIRE_EQ(result_point, res.end_point());
}

void test_intersection_in_common_endpoint()
{
    gkernel::Point point_a1(1, 5), point_b1(3, 1);
    gkernel::Point point_a2(1, 5), point_b2(5, 5);
    gkernel::Point result_point(1, 5);

    gkernel::Segment s1(point_a1, point_b1);
    gkernel::Segment s2(point_a2, point_b2);
    gkernel::Segment res = gkernel::pointintersection(s1, s2);

    REQUIRE_EQ(result_point, res.begin_point());
    REQUIRE_EQ(result_point, res.end_point());
}

void test_endpoint_on_segment_intersection()
{
    gkernel::Point point_a1(1, 5), point_b1(3, 1);
    gkernel::Point point_a2(0, 4), point_b2(2, 3);
    gkernel::Point result_point(2, 3);

    gkernel::Segment s1(point_a1, point_b1);
    gkernel::Segment s2(point_a2, point_b2);
    gkernel::Segment res = gkernel::pointintersection(s1, s2);

    REQUIRE_EQ(result_point, res.begin_point());
    REQUIRE_EQ(result_point, res.end_point());
}

void test_segments_overlay()
{
    gkernel::Point point_a1(1, 5), point_b1(3, 1);
    gkernel::Point point_a2(2, 3), point_b2(4, -1);
    gkernel::Point begin_point(2, 3), end_point(3, 1);

    gkernel::Segment s1(point_a1, point_b1);
    gkernel::Segment s2(point_a2, point_b2);
    gkernel::Segment res = gkernel::pointintersection(s1, s2);

    REQUIRE_EQ(begin_point, res.begin_point());
    REQUIRE_EQ(end_point, res.end_point());
}

void test_segments_full_overlay()
{
    gkernel::Point point_a1(1, 5), point_b1(3, 1);
    gkernel::Point point_a2(1, 5), point_b2(3, 1);
    gkernel::Point begin_point(1, 5), end_point(3, 1);

    gkernel::Segment s1(point_a1, point_b1);
    gkernel::Segment s2(point_a2, point_b2);
    gkernel::Segment res = gkernel::pointintersection(s1, s2);

    REQUIRE_EQ(begin_point, res.begin_point());
    REQUIRE_EQ(end_point, res.end_point());
}

void test_orthogonal_intersection()
{
    gkernel::Point point_a1(1, 3), point_b1(5, 3);
    gkernel::Point point_a2(3, 1), point_b2(3, 5);
    gkernel::Point begin_point(3, 3), end_point(3, 3);

    gkernel::Segment s1(point_a1, point_b1);
    gkernel::Segment s2(point_a2, point_b2);
    gkernel::Segment res = gkernel::pointintersection(s1, s2);

    REQUIRE_EQ(begin_point, res.begin_point());
    REQUIRE_EQ(end_point, res.end_point());
}

void test_solve_1()
{
    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(gkernel::Point(3.5, 5.5), gkernel::Point(4, 1));
    segvec.emplace_back(gkernel::Point(3.5, 5.5), gkernel::Point(6, 3));
    segvec.emplace_back(gkernel::Point(4, 3), gkernel::Point(6, 1));
    segvec.emplace_back(gkernel::Point(6, 1), gkernel::Point(10, 5));
    segvec.emplace_back(gkernel::Point(4, 3), gkernel::Point(6, 5));
    segvec.emplace_back(gkernel::Point(4, 1), gkernel::Point(6, 3));
    segvec.emplace_back(gkernel::Point(6, 5), gkernel::Point(9, 6));
    segvec.emplace_back(gkernel::Point(9, 6), gkernel::Point(10, 5));
    segvec.emplace_back(gkernel::Point(6, 4), gkernel::Point(9, 5));
    segvec.emplace_back(gkernel::Point(6, 4), gkernel::Point(8, 0));
    segvec.emplace_back(gkernel::Point(9, 5), gkernel::Point(11, 7));
    segvec.emplace_back(gkernel::Point(8, 0), gkernel::Point(13, 5));
    segvec.emplace_back(gkernel::Point(10, 4), gkernel::Point(13, 7));
    segvec.emplace_back(gkernel::Point(11, 7), gkernel::Point(13, 5));
    segvec.emplace_back(gkernel::Point(10, 4), gkernel::Point(12, 2));
    segvec.emplace_back(gkernel::Point(11, 2), gkernel::Point(13, 4));
    segvec.emplace_back(gkernel::Point(11, 2), gkernel::Point(12, 1));
    segvec.emplace_back(gkernel::Point(12, 1), gkernel::Point(15, 2));
    segvec.emplace_back(gkernel::Point(12, 2), gkernel::Point(16, 4));
    segvec.emplace_back(gkernel::Point(13, 4), gkernel::Point(15, 2));
    segvec.emplace_back(gkernel::Point(13, 7), gkernel::Point(14, 5));
    segvec.emplace_back(gkernel::Point(14, 5), gkernel::Point(16, 4));

    std::set<gkernel::Segment> expected;
    expected.emplace(gkernel::Point(3.5, 5.5), gkernel::Point(3.5, 5.5));
    expected.emplace(gkernel::Point(4, 3), gkernel::Point(4, 3));
    expected.emplace(gkernel::Point(4, 1), gkernel::Point(4, 1));
    expected.emplace(gkernel::Point(5, 4), gkernel::Point(5, 4));
    expected.emplace(gkernel::Point(5, 2), gkernel::Point(5, 2));
    expected.emplace(gkernel::Point(6, 5), gkernel::Point(6, 5));
    expected.emplace(gkernel::Point(6, 4), gkernel::Point(6, 4));
    expected.emplace(gkernel::Point(6, 3), gkernel::Point(6, 3));
    expected.emplace(gkernel::Point(6, 1), gkernel::Point(6, 1));
    expected.emplace(gkernel::Point(7, 2), gkernel::Point(7, 2));
    expected.emplace(gkernel::Point(8, 0), gkernel::Point(8, 0));
    expected.emplace(gkernel::Point(9, 6), gkernel::Point(9, 6));
    expected.emplace(gkernel::Point(9, 5), gkernel::Point(9, 5));
    expected.emplace(gkernel::Point(9.5, 5.5), gkernel::Point(9.5, 5.5));
    expected.emplace(gkernel::Point(10, 5), gkernel::Point(10, 5));
    expected.emplace(gkernel::Point(10, 4), gkernel::Point(10, 4));
    expected.emplace(gkernel::Point(11, 7), gkernel::Point(11, 7));
    expected.emplace(gkernel::Point(11, 3), gkernel::Point(11, 3));
    expected.emplace(gkernel::Point(11, 2), gkernel::Point(11, 2));
    expected.emplace(gkernel::Point(11.5, 2.5), gkernel::Point(11.5, 2.5));
    expected.emplace(gkernel::Point(12, 6), gkernel::Point(12, 6));
    expected.emplace(gkernel::Point(12, 2), gkernel::Point(12, 2));
    expected.emplace(gkernel::Point(12, 1), gkernel::Point(12, 1));
    expected.emplace(gkernel::Point(13, 7), gkernel::Point(13, 7));
    expected.emplace(gkernel::Point(13, 5), gkernel::Point(13, 5));
    expected.emplace(gkernel::Point(13, 4), gkernel::Point(13, 4));
    expected.emplace(gkernel::Point(14, 5), gkernel::Point(14, 5));
    expected.emplace(gkernel::Point(14, 3), gkernel::Point(14, 3));
    expected.emplace(gkernel::Point(15, 2), gkernel::Point(15, 2));
    expected.emplace(gkernel::Point(16, 4), gkernel::Point(16, 4));

    std::set<gkernel::Segment> res = gkernel::solve(segvec);
    REQUIRE_EQ(res, expected);
}

void test_solve_2()
{
    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(gkernel::Point(2, 2), gkernel::Point(9, 9));
    segvec.emplace_back(gkernel::Point(3, 9), gkernel::Point(9, 3));
    segvec.emplace_back(gkernel::Point(2, 5), gkernel::Point(6, 6));
    segvec.emplace_back(gkernel::Point(6, 10), gkernel::Point(14, 2));
    segvec.emplace_back(gkernel::Point(6.5, 2), gkernel::Point(8, 4));
    segvec.emplace_back(gkernel::Point(6.5, 2), gkernel::Point(9, 3));
    segvec.emplace_back(gkernel::Point(2, 5), gkernel::Point(3, 9));
    segvec.emplace_back(gkernel::Point(2, 2), gkernel::Point(10, 1));
    segvec.emplace_back(gkernel::Point(10, 1), gkernel::Point(14, 2));
    segvec.emplace_back(gkernel::Point(6, 10), gkernel::Point(9, 9));
    segvec.emplace_back(gkernel::Point(10, 1), gkernel::Point(16, 5));
    segvec.emplace_back(gkernel::Point(11, 10), gkernel::Point(19, 2));
    segvec.emplace_back(gkernel::Point(13, 6), gkernel::Point(17, 10));
    segvec.emplace_back(gkernel::Point(16, 7), gkernel::Point(18, 3));
    segvec.emplace_back(gkernel::Point(11, 10), gkernel::Point(21, 8));
    segvec.emplace_back(gkernel::Point(16, 7), gkernel::Point(21, 8));
    segvec.emplace_back(gkernel::Point(17, 2), gkernel::Point(22, 7));
    segvec.emplace_back(gkernel::Point(21, 8), gkernel::Point(22, 7));
    segvec.emplace_back(gkernel::Point(2, 5), gkernel::Point(4, 3));
    segvec.emplace_back(gkernel::Point(4, 3), gkernel::Point(6.5, 2));

    std::set<gkernel::Segment> expected;
    expected.emplace(gkernel::Point(2, 2), gkernel::Point(2, 2));
    expected.emplace(gkernel::Point(2, 5), gkernel::Point(2, 5));
    expected.emplace(gkernel::Point(3, 9), gkernel::Point(3, 9));
    expected.emplace(gkernel::Point(3.5, 3.5), gkernel::Point(3.5, 3.5));
    expected.emplace(gkernel::Point(4, 3), gkernel::Point(4, 3));
    expected.emplace(gkernel::Point(6, 6), gkernel::Point(6, 6));
    expected.emplace(gkernel::Point(6, 10), gkernel::Point(6, 10));
    expected.emplace(gkernel::Point(6.5, 2), gkernel::Point(6.5, 2));
    expected.emplace(gkernel::Point(8, 8), gkernel::Point(8, 8));
    expected.emplace(gkernel::Point(8, 4), gkernel::Point(8, 4));
    expected.emplace(gkernel::Point(9, 9), gkernel::Point(9, 9));
    expected.emplace(gkernel::Point(9, 3), gkernel::Point(9, 3));
    expected.emplace(gkernel::Point(10, 1), gkernel::Point(10, 1));
    expected.emplace(gkernel::Point(11, 10), gkernel::Point(11, 10));
    expected.emplace(gkernel::Point(13, 3), gkernel::Point(13, 3));
    expected.emplace(gkernel::Point(14, 7), gkernel::Point(14, 7));
    expected.emplace(gkernel::Point(14, 2), gkernel::Point(14, 2));
    expected.emplace(gkernel::Point(16, 9), gkernel::Point(16, 9));
    expected.emplace(gkernel::Point(16, 7), gkernel::Point(16, 7));
    expected.emplace(gkernel::Point(16, 5), gkernel::Point(16, 5));
    expected.emplace(gkernel::Point(18, 3), gkernel::Point(18, 3));
    expected.emplace(gkernel::Point(21, 8), gkernel::Point(21, 8));
    expected.emplace(gkernel::Point(22, 7), gkernel::Point(22, 7));

    std::set<gkernel::Segment> res = gkernel::solve(segvec);
    REQUIRE_EQ(res, expected);
}

void test_solve_3()
{
    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(gkernel::Point(2, 1), gkernel::Point(6, 5));
    segvec.emplace_back(gkernel::Point(6, 5), gkernel::Point(7, 5));
    segvec.emplace_back(gkernel::Point(7, 5), gkernel::Point(8, 1));
    segvec.emplace_back(gkernel::Point(2, 1), gkernel::Point(8, 1));
    segvec.emplace_back(gkernel::Point(2, 7), gkernel::Point(9, 0));
    segvec.emplace_back(gkernel::Point(2, 7), gkernel::Point(8, 7));
    segvec.emplace_back(gkernel::Point(8, 7), gkernel::Point(9, 0));
    segvec.emplace_back(gkernel::Point(9, 6), gkernel::Point(10, 8));
    segvec.emplace_back(gkernel::Point(10, 8), gkernel::Point(12, 7));
    segvec.emplace_back(gkernel::Point(12, 7), gkernel::Point(13, 5));
    segvec.emplace_back(gkernel::Point(13, 5), gkernel::Point(15, 3));
    segvec.emplace_back(gkernel::Point(12, 3), gkernel::Point(15, 3));
    segvec.emplace_back(gkernel::Point(9, 6), gkernel::Point(12, 3));
    segvec.emplace_back(gkernel::Point(12, 5), gkernel::Point(18, 5));
    segvec.emplace_back(gkernel::Point(10, 8), gkernel::Point(18, 8));
    segvec.emplace_back(gkernel::Point(13, 10), gkernel::Point(18, 5));
    segvec.emplace_back(gkernel::Point(11, 9), gkernel::Point(21, 9));
    segvec.emplace_back(gkernel::Point(11, 9), gkernel::Point(13, 10));
    segvec.emplace_back(gkernel::Point(18, 8), gkernel::Point(21, 9));
    segvec.emplace_back(gkernel::Point(10, 8), gkernel::Point(12, 5));

    std::set<gkernel::Segment> expected;
    expected.emplace(gkernel::Point(2, 7), gkernel::Point(2, 7));
    expected.emplace(gkernel::Point(2, 1), gkernel::Point(2, 1));
    expected.emplace(gkernel::Point(5, 4), gkernel::Point(5, 4));
    expected.emplace(gkernel::Point(6, 5), gkernel::Point(6, 5));
    expected.emplace(gkernel::Point(7, 5), gkernel::Point(7, 5));
    expected.emplace(gkernel::Point(8, 1), gkernel::Point(8, 1));
    expected.emplace(gkernel::Point(8, 7), gkernel::Point(8, 7));
    expected.emplace(gkernel::Point(9, 0), gkernel::Point(9, 0));
    expected.emplace(gkernel::Point(9, 6), gkernel::Point(9, 6));
    expected.emplace(gkernel::Point(10, 8), gkernel::Point(10, 8));
    expected.emplace(gkernel::Point(11, 9), gkernel::Point(11, 9));
    expected.emplace(gkernel::Point(12, 7), gkernel::Point(12, 7));
    expected.emplace(gkernel::Point(12, 5), gkernel::Point(12, 5));
    expected.emplace(gkernel::Point(12, 3), gkernel::Point(12, 3));
    expected.emplace(gkernel::Point(13, 10), gkernel::Point(13, 10));
    expected.emplace(gkernel::Point(13, 5), gkernel::Point(13, 5));
    expected.emplace(gkernel::Point(14, 9), gkernel::Point(14, 9));
    expected.emplace(gkernel::Point(15, 8), gkernel::Point(15, 8));
    expected.emplace(gkernel::Point(15, 3), gkernel::Point(15, 3));
    expected.emplace(gkernel::Point(18, 8), gkernel::Point(18, 8));
    expected.emplace(gkernel::Point(18, 5), gkernel::Point(18, 5));
    expected.emplace(gkernel::Point(21, 9), gkernel::Point(21, 9));

    std::set<gkernel::Segment> res = gkernel::solve(segvec);
    REQUIRE_EQ(res, expected);
}

void test_solve_4()
{
    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(gkernel::Point(3, 12), gkernel::Point(6, 2));
    segvec.emplace_back(gkernel::Point(3, 12), gkernel::Point(10, 5));
    segvec.emplace_back(gkernel::Point(6, 2), gkernel::Point(10, 4));
    segvec.emplace_back(gkernel::Point(6, 12), gkernel::Point(6, 2));
    segvec.emplace_back(gkernel::Point(6, 12), gkernel::Point(14, 4));
    segvec.emplace_back(gkernel::Point(10, 11), gkernel::Point(10, 0));
    segvec.emplace_back(gkernel::Point(10, 11), gkernel::Point(16, 8));
    segvec.emplace_back(gkernel::Point(10, 5), gkernel::Point(16, 8));
    segvec.emplace_back(gkernel::Point(10, 0), gkernel::Point(14, 4));
    segvec.emplace_back(gkernel::Point(14, 12), gkernel::Point(14, 2));
    segvec.emplace_back(gkernel::Point(14, 12), gkernel::Point(22, 6));
    segvec.emplace_back(gkernel::Point(16, 8), gkernel::Point(20, 0));
    segvec.emplace_back(gkernel::Point(14, 2), gkernel::Point(22, 6));
    segvec.emplace_back(gkernel::Point(22, 6), gkernel::Point(24, 10));
    segvec.emplace_back(gkernel::Point(22, 6), gkernel::Point(26, 2));
    segvec.emplace_back(gkernel::Point(20, 0), gkernel::Point(28, 8));
    segvec.emplace_back(gkernel::Point(24, 10), gkernel::Point(24, 0));
    segvec.emplace_back(gkernel::Point(24, 10), gkernel::Point(26, 6));
    segvec.emplace_back(gkernel::Point(24, 0), gkernel::Point(26, 2));
    segvec.emplace_back(gkernel::Point(26, 2), gkernel::Point(28, 8));

    std::set<gkernel::Segment> expected;
    expected.emplace(gkernel::Point(3, 12), gkernel::Point(3, 12));
    expected.emplace(gkernel::Point(6, 2), gkernel::Point(6, 2));
    expected.emplace(gkernel::Point(6, 9), gkernel::Point(6, 9));
    expected.emplace(gkernel::Point(6, 12), gkernel::Point(6, 12));
    expected.emplace(gkernel::Point(10, 11), gkernel::Point(10, 11));
    expected.emplace(gkernel::Point(10, 5), gkernel::Point(10, 5));
    expected.emplace(gkernel::Point(10, 4), gkernel::Point(10, 4));
    expected.emplace(gkernel::Point(10, 0), gkernel::Point(10, 0));
    expected.emplace(gkernel::Point(10, 8), gkernel::Point(10, 8));
    expected.emplace(gkernel::Point(12, 6), gkernel::Point(12, 6));
    expected.emplace(gkernel::Point(14, 12), gkernel::Point(14, 12));
    expected.emplace(gkernel::Point(14, 9), gkernel::Point(14, 9));
    expected.emplace(gkernel::Point(14, 7), gkernel::Point(14, 7));
    expected.emplace(gkernel::Point(14, 4), gkernel::Point(14, 4));
    expected.emplace(gkernel::Point(14, 2), gkernel::Point(14, 2));
    expected.emplace(gkernel::Point(16, 8), gkernel::Point(16, 8));
    expected.emplace(gkernel::Point(18, 4), gkernel::Point(18, 4));
    expected.emplace(gkernel::Point(20, 0), gkernel::Point(20, 0));
    expected.emplace(gkernel::Point(22, 6), gkernel::Point(22, 6));
    expected.emplace(gkernel::Point(24, 10), gkernel::Point(24, 10));
    expected.emplace(gkernel::Point(24, 4), gkernel::Point(24, 4));
    expected.emplace(gkernel::Point(24, 0), gkernel::Point(24, 0));
    expected.emplace(gkernel::Point(26, 2), gkernel::Point(26, 2));
    expected.emplace(gkernel::Point(26, 6), gkernel::Point(26, 6));
    expected.emplace(gkernel::Point(28, 8), gkernel::Point(28, 8));

    std::set<gkernel::Segment> res = gkernel::solve(segvec);
    REQUIRE_EQ(res, expected);
}

void test_solve_5()
{
    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(gkernel::Point(1, 6), gkernel::Point(1, 2));
    segvec.emplace_back(gkernel::Point(1, 6), gkernel::Point(10, 6));
    segvec.emplace_back(gkernel::Point(1, 2), gkernel::Point(8, 2));
    segvec.emplace_back(gkernel::Point(2, 8), gkernel::Point(2, 1));
    segvec.emplace_back(gkernel::Point(6, 8), gkernel::Point(6, 1));
    segvec.emplace_back(gkernel::Point(2, 8), gkernel::Point(6, 8));
    segvec.emplace_back(gkernel::Point(2, 1), gkernel::Point(6, 1));
    segvec.emplace_back(gkernel::Point(8, 9), gkernel::Point(8, 2));
    segvec.emplace_back(gkernel::Point(6, 8), gkernel::Point(8, 9));
    segvec.emplace_back(gkernel::Point(10, 4), gkernel::Point(16, 4));
    segvec.emplace_back(gkernel::Point(10, 6), gkernel::Point(10, 4));
    segvec.emplace_back(gkernel::Point(15, 8), gkernel::Point(15, 2));
    segvec.emplace_back(gkernel::Point(8, 2), gkernel::Point(15, 2));
    segvec.emplace_back(gkernel::Point(8, 9), gkernel::Point(15, 8));
    segvec.emplace_back(gkernel::Point(16, 7), gkernel::Point(16, 4));
    segvec.emplace_back(gkernel::Point(13, 7), gkernel::Point(16, 7));
    segvec.emplace_back(gkernel::Point(13, 7), gkernel::Point(13, 5));
    segvec.emplace_back(gkernel::Point(13, 5), gkernel::Point(15, 5));
    segvec.emplace_back(gkernel::Point(2, 6), gkernel::Point(6, 2));
    segvec.emplace_back(gkernel::Point(2, 2), gkernel::Point(6, 6));

    std::set<gkernel::Segment> expected;
    expected.emplace(gkernel::Point(1, 6), gkernel::Point(1, 6));
    expected.emplace(gkernel::Point(1, 2), gkernel::Point(1, 2));
    expected.emplace(gkernel::Point(2, 8), gkernel::Point(2, 8));
    expected.emplace(gkernel::Point(2, 6), gkernel::Point(2, 6));
    expected.emplace(gkernel::Point(2, 2), gkernel::Point(2, 2));
    expected.emplace(gkernel::Point(2, 1), gkernel::Point(2, 1));
    expected.emplace(gkernel::Point(4, 4), gkernel::Point(4, 4));
    expected.emplace(gkernel::Point(6, 8), gkernel::Point(6, 8));
    expected.emplace(gkernel::Point(6, 6), gkernel::Point(6, 6));
    expected.emplace(gkernel::Point(6, 2), gkernel::Point(6, 2));
    expected.emplace(gkernel::Point(6, 1), gkernel::Point(6, 1));
    expected.emplace(gkernel::Point(8, 9), gkernel::Point(8, 9));
    expected.emplace(gkernel::Point(8, 6), gkernel::Point(8, 6));
    expected.emplace(gkernel::Point(8, 2), gkernel::Point(8, 2));
    expected.emplace(gkernel::Point(10, 6), gkernel::Point(10, 6));
    expected.emplace(gkernel::Point(10, 4), gkernel::Point(10, 4));
    expected.emplace(gkernel::Point(13, 7), gkernel::Point(13, 7));
    expected.emplace(gkernel::Point(13, 5), gkernel::Point(13, 5));
    expected.emplace(gkernel::Point(15, 8), gkernel::Point(15, 8));
    expected.emplace(gkernel::Point(15, 7), gkernel::Point(15, 7));
    expected.emplace(gkernel::Point(15, 5), gkernel::Point(15, 5));
    expected.emplace(gkernel::Point(15, 4), gkernel::Point(15, 4));
    expected.emplace(gkernel::Point(15, 2), gkernel::Point(15, 2));
    expected.emplace(gkernel::Point(16, 7), gkernel::Point(16, 7));
    expected.emplace(gkernel::Point(16, 4), gkernel::Point(16, 4));

    std::set<gkernel::Segment> res = gkernel::solve(segvec);
    REQUIRE_EQ(res, expected);
}

void test_solve_6()
{
    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(gkernel::Point(1, 6), gkernel::Point(1, 2));
    segvec.emplace_back(gkernel::Point(2, 10), gkernel::Point(2, 2));
    segvec.emplace_back(gkernel::Point(3, 8), gkernel::Point(3, 4));
    segvec.emplace_back(gkernel::Point(5, 11), gkernel::Point(5, 5));
    segvec.emplace_back(gkernel::Point(8, 8), gkernel::Point(8, 5));
    segvec.emplace_back(gkernel::Point(8, 5), gkernel::Point(8, 3));
    segvec.emplace_back(gkernel::Point(9, 10), gkernel::Point(9, 4));
    segvec.emplace_back(gkernel::Point(11, 5), gkernel::Point(11, 3));
    segvec.emplace_back(gkernel::Point(12, 11), gkernel::Point(12, 6));
    segvec.emplace_back(gkernel::Point(1, 2), gkernel::Point(2, 2));
    segvec.emplace_back(gkernel::Point(8, 3), gkernel::Point(11, 3));
    segvec.emplace_back(gkernel::Point(3, 4), gkernel::Point(9, 4));
    segvec.emplace_back(gkernel::Point(5, 5), gkernel::Point(8, 5));
    segvec.emplace_back(gkernel::Point(8, 5), gkernel::Point(11, 5));
    segvec.emplace_back(gkernel::Point(1, 6), gkernel::Point(12, 6));
    segvec.emplace_back(gkernel::Point(3, 8), gkernel::Point(8, 8));
    segvec.emplace_back(gkernel::Point(2, 10), gkernel::Point(9, 10));
    segvec.emplace_back(gkernel::Point(5, 11), gkernel::Point(12, 11));

    std::set<gkernel::Segment> expected;
    expected.emplace(gkernel::Point(1, 6), gkernel::Point(1, 6));
    expected.emplace(gkernel::Point(1, 2), gkernel::Point(1, 2));
    expected.emplace(gkernel::Point(2, 10), gkernel::Point(2, 10));
    expected.emplace(gkernel::Point(2, 6), gkernel::Point(2, 6));
    expected.emplace(gkernel::Point(2, 2), gkernel::Point(2, 2));
    expected.emplace(gkernel::Point(3, 8), gkernel::Point(3, 8));
    expected.emplace(gkernel::Point(3, 6), gkernel::Point(3, 6));
    expected.emplace(gkernel::Point(3, 4), gkernel::Point(3, 4));
    expected.emplace(gkernel::Point(5, 11), gkernel::Point(5, 11));
    expected.emplace(gkernel::Point(5, 10), gkernel::Point(5, 10));
    expected.emplace(gkernel::Point(5, 8), gkernel::Point(5, 8));
    expected.emplace(gkernel::Point(5, 6), gkernel::Point(5, 6));
    expected.emplace(gkernel::Point(5, 5), gkernel::Point(5, 5));
    expected.emplace(gkernel::Point(8, 8), gkernel::Point(8, 8));
    expected.emplace(gkernel::Point(8, 6), gkernel::Point(8, 6));
    expected.emplace(gkernel::Point(8, 5), gkernel::Point(8, 5));
    expected.emplace(gkernel::Point(8, 4), gkernel::Point(8, 4));
    expected.emplace(gkernel::Point(8, 3), gkernel::Point(8, 3));
    expected.emplace(gkernel::Point(9, 10), gkernel::Point(9, 10));
    expected.emplace(gkernel::Point(9, 6), gkernel::Point(9, 6));
    expected.emplace(gkernel::Point(9, 5), gkernel::Point(9, 5));
    expected.emplace(gkernel::Point(9, 4), gkernel::Point(9, 4));
    expected.emplace(gkernel::Point(11, 5), gkernel::Point(11, 5));
    expected.emplace(gkernel::Point(11, 3), gkernel::Point(11, 3));
    expected.emplace(gkernel::Point(12, 11), gkernel::Point(12, 11));
    expected.emplace(gkernel::Point(12, 6), gkernel::Point(12, 6));

    std::set<gkernel::Segment> res = gkernel::solve(segvec);
    REQUIRE_EQ(res, expected);
}
void test_solve_7()
{
    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(gkernel::Point(2, 2), gkernel::Point(7, 7));
    segvec.emplace_back(gkernel::Point(2, 2), gkernel::Point(7, 7));
    segvec.emplace_back(gkernel::Point(2, 4), gkernel::Point(5, 1));
    segvec.emplace_back(gkernel::Point(3, 3), gkernel::Point(5, 1));
    segvec.emplace_back(gkernel::Point(3, 5), gkernel::Point(6, 2));
    segvec.emplace_back(gkernel::Point(4, 10), gkernel::Point(7, 7));
    segvec.emplace_back(gkernel::Point(5, 9), gkernel::Point(6, 8));
    segvec.emplace_back(gkernel::Point(5, 9), gkernel::Point(6, 8));
    segvec.emplace_back(gkernel::Point(5, 7), gkernel::Point(11, 1));
    segvec.emplace_back(gkernel::Point(8, 4), gkernel::Point(10, 2));
    segvec.emplace_back(gkernel::Point(8, 4), gkernel::Point(10, 2));
    segvec.emplace_back(gkernel::Point(6, 2), gkernel::Point(10, 6));
    segvec.emplace_back(gkernel::Point(8, 6), gkernel::Point(12, 2));
    segvec.emplace_back(gkernel::Point(9, 1), gkernel::Point(13, 5));
    segvec.emplace_back(gkernel::Point(11, 5), gkernel::Point(14, 2));
    segvec.emplace_back(gkernel::Point(11, 1), gkernel::Point(15, 5));
    segvec.emplace_back(gkernel::Point(13, 3), gkernel::Point(15, 5));
    segvec.emplace_back(gkernel::Point(13, 3), gkernel::Point(15, 5));

    std::set<gkernel::Segment> expected;
    expected.emplace(gkernel::Point(3, 3), gkernel::Point(3, 3));
    expected.emplace(gkernel::Point(3, 3), gkernel::Point(5, 1));
    expected.emplace(gkernel::Point(2, 2), gkernel::Point(7, 7));
    expected.emplace(gkernel::Point(5, 9), gkernel::Point(6, 8));
    expected.emplace(gkernel::Point(4, 4), gkernel::Point(4, 4));
    expected.emplace(gkernel::Point(6, 6), gkernel::Point(6, 6));
    expected.emplace(gkernel::Point(6, 2), gkernel::Point(6, 2));
    expected.emplace(gkernel::Point(7, 7), gkernel::Point(7, 7));
    expected.emplace(gkernel::Point(8, 4), gkernel::Point(10, 2));
    expected.emplace(gkernel::Point(8, 4), gkernel::Point(8, 4));
    expected.emplace(gkernel::Point(9, 5), gkernel::Point(9, 5));
    expected.emplace(gkernel::Point(10, 2), gkernel::Point(10, 2));
    expected.emplace(gkernel::Point(11, 3), gkernel::Point(11, 3));
    expected.emplace(gkernel::Point(11, 1), gkernel::Point(11, 1));
    expected.emplace(gkernel::Point(12, 4), gkernel::Point(12, 4));
    expected.emplace(gkernel::Point(12, 2), gkernel::Point(12, 2));
    expected.emplace(gkernel::Point(13, 3), gkernel::Point(13, 3));
    expected.emplace(gkernel::Point(13, 3), gkernel::Point(15, 5));

    std::set<gkernel::Segment> res = gkernel::solve(segvec);
    REQUIRE_EQ(res, expected);
}
void test_solve_8()
{
    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(gkernel::Point(1, 1), gkernel::Point(6, 6));
    segvec.emplace_back(gkernel::Point(1, 1), gkernel::Point(6, 6));
    segvec.emplace_back(gkernel::Point(1, 1), gkernel::Point(6, 6));
    segvec.emplace_back(gkernel::Point(1, 1), gkernel::Point(6, 1));
    segvec.emplace_back(gkernel::Point(6, 6), gkernel::Point(6, 1));
    segvec.emplace_back(gkernel::Point(1, 1), gkernel::Point(6, 6));
    segvec.emplace_back(gkernel::Point(1, 1), gkernel::Point(6, 1));
    segvec.emplace_back(gkernel::Point(6, 6), gkernel::Point(6, 1));
    segvec.emplace_back(gkernel::Point(4, 2), gkernel::Point(4, 7));
    segvec.emplace_back(gkernel::Point(4, 7), gkernel::Point(11, 7));
    segvec.emplace_back(gkernel::Point(4, 2), gkernel::Point(11, 2));
    segvec.emplace_back(gkernel::Point(11, 7), gkernel::Point(11, 2));
    segvec.emplace_back(gkernel::Point(10, 7), gkernel::Point(11, 7));
    segvec.emplace_back(gkernel::Point(10, 7), gkernel::Point(11, 7));
    segvec.emplace_back(gkernel::Point(11, 7), gkernel::Point(11, 6));
    segvec.emplace_back(gkernel::Point(11, 7), gkernel::Point(11, 6));
    segvec.emplace_back(gkernel::Point(11, 7), gkernel::Point(11, 6));
    segvec.emplace_back(gkernel::Point(8, 2), gkernel::Point(10, 2));
    segvec.emplace_back(gkernel::Point(8, 5), gkernel::Point(8, 2));
    segvec.emplace_back(gkernel::Point(8, 5), gkernel::Point(10, 5));
    segvec.emplace_back(gkernel::Point(10, 5), gkernel::Point(10, 2));

    std::set<gkernel::Segment> expected;
    expected.emplace(gkernel::Point(1, 1), gkernel::Point(1, 1));
    expected.emplace(gkernel::Point(4, 7), gkernel::Point(4, 7));
    expected.emplace(gkernel::Point(4, 4), gkernel::Point(4, 4));
    expected.emplace(gkernel::Point(4, 2), gkernel::Point(4, 2));
    expected.emplace(gkernel::Point(6, 6), gkernel::Point(6, 6));
    expected.emplace(gkernel::Point(6, 2), gkernel::Point(6, 2));
    expected.emplace(gkernel::Point(6, 1), gkernel::Point(6, 1));
    expected.emplace(gkernel::Point(8, 5), gkernel::Point(8, 5));
    expected.emplace(gkernel::Point(10, 5), gkernel::Point(10, 5));
    expected.emplace(gkernel::Point(11, 7), gkernel::Point(11, 7));
    expected.emplace(gkernel::Point(11, 2), gkernel::Point(11, 2));
    segvec.emplace_back(gkernel::Point(1, 1), gkernel::Point(6, 6));
    segvec.emplace_back(gkernel::Point(1, 1), gkernel::Point(6, 1));
    segvec.emplace_back(gkernel::Point(6, 6), gkernel::Point(6, 1));
    segvec.emplace_back(gkernel::Point(8, 2), gkernel::Point(10, 2));
    segvec.emplace_back(gkernel::Point(10, 7), gkernel::Point(11, 7));
    segvec.emplace_back(gkernel::Point(11, 7), gkernel::Point(11, 6));

    std::set<gkernel::Segment> res = gkernel::solve(segvec);
    REQUIRE_EQ(res, expected);
}

TEST_CASE("no intersection")
{
    test_no_intersection();
}

TEST_CASE("intersection in inner point")
{
    test_inner_intersection();
}

TEST_CASE("intersection in common endpoint")
{
    test_intersection_in_common_endpoint();
}

TEST_CASE("endpoint lies on segment")
{
    test_endpoint_on_segment_intersection();
}

TEST_CASE("segments overlay")
{
    test_segments_overlay();
}

TEST_CASE("segments full overlay")
{
    test_segments_full_overlay();
}

TEST_CASE("orthogonal intersection")
{
    test_orthogonal_intersection();
}

TEST_CASE("test 1")
{
    test_solve_1();
}

TEST_CASE("test 2")
{
    test_solve_2();
}

TEST_CASE("test 3")
{
    test_solve_3();
}

TEST_CASE("test 4")
{
    test_solve_4();
}

TEST_CASE("test 5")
{
    test_solve_5();
}

TEST_CASE("test 6")
{
    test_solve_6();
}

TEST_CASE("test 7")
{
    test_solve_7();
}

TEST_CASE("test 8")
{
    test_solve_8();
}