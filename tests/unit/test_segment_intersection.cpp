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

// intersections in endpoints and inner area, no special case
void test_solve_simple()
{
    gkernel::Point p_b1(1, 5), p_e1(4, 2);
    gkernel::Point p_b2(1, 1), p_e2(5, 5);
    gkernel::Point p_b3(4, 2), p_e3(8, 6);
    gkernel::Point p_b4(5, 5), p_e4(9, 1);

    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(p_b1, p_e1);
    segvec.emplace_back(p_b2, p_e2);
    segvec.emplace_back(p_b3, p_e3);
    segvec.emplace_back(p_b4, p_e4);

    gkernel::Point res_p1(3, 3), res_p2(4, 2), res_p3(5, 5), res_p4(6, 4);
    gkernel::Segment res_s1(res_p1, res_p1), res_s2(res_p2, res_p2), res_s3(res_p3, res_p3), res_s4(res_p4, res_p4);

    std::set<gkernel::Segment> expected{res_s1, res_s2, res_s3, res_s4};
    std::set<gkernel::Segment> res = gkernel::solve(segvec);

    REQUIRE_EQ(res, expected);
}

// 2 segments complete and partial overlay and 1 intersection point
void test_solve_overlay()
{
    gkernel::Point p_b1(1, 1), p_e1(4, 4);
    gkernel::Point p_b2(4, 4), p_e2(7, 1);
    gkernel::Point p_b3(7, 1), p_e3(8, 4);
    gkernel::Point p_b4(1, 1), p_e4(4, 4);
    gkernel::Point p_b5(4, 4), p_e5(6, 2);
    gkernel::Point p_b6(6, 2), p_e6(9, 5);

    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(p_b1, p_e1);
    segvec.emplace_back(p_b2, p_e2);
    segvec.emplace_back(p_b3, p_e3);
    segvec.emplace_back(p_b4, p_e4);
    segvec.emplace_back(p_b5, p_e5);
    segvec.emplace_back(p_b6, p_e6);

    gkernel::Point res_p1(1, 1), res_p2(4, 4), res_p3(6, 2), res_p4(8, 4);
    gkernel::Segment res_s1(res_p1, res_p2), res_s2(res_p2, res_p3), res_s3(res_p4, res_p4);

    std::set<gkernel::Segment> expected{res_s1, res_s2, res_s3};
    std::set<gkernel::Segment> res = gkernel::solve(segvec);

    REQUIRE_EQ(res, expected);
}

void test_solve_3_segment_intersection()
{
    gkernel::Point p_b1(0, 0), p_e1(4, 4);
    gkernel::Point p_b2(0, 4), p_e2(4, 0);
    gkernel::Point p_b3(0, 3), p_e3(2, 2);

    std::vector<gkernel::Segment> segvec;
    segvec.emplace_back(p_b1, p_e1);
    segvec.emplace_back(p_b2, p_e2);
    segvec.emplace_back(p_b3, p_e3);

    gkernel::Point res_p1(2, 2);
    gkernel::Segment res_s1(res_p1, res_p1);

    std::set<gkernel::Segment> expected{res_s1};
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

TEST_CASE("simple segment set intersection")
{
    test_solve_simple();
}

TEST_CASE("segment set overlay")
{
    test_solve_overlay();
}

TEST_CASE("3 segments intersect in one point")
{
    test_solve_3_segment_intersection();
}