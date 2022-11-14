#include "test.hpp"
#include "gkernel/objects.hpp"
#include "gkernel/intersection.hpp"
#include "gkernel/rbtree.hpp"
#include <vector>
#include <set>

using namespace gkernel;
using RBtree = gkernel::RBTree<gkernel::Segment, std::function<bool(const Segment& c1, const Segment& c2)>>;

void test_1() {
    SegmentsSet segvec;
    //segvec.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(4, 1)});
    //segvec.emplace_back({gkernel::Point(3.5, 5.5), gkernel::Point(6, 3)});
    //segvec.emplace_back({gkernel::Point(4, 3), gkernel::Point(6, 1)});
    //segvec.emplace_back({gkernel::Point(4, 3), gkernel::Point(6, 5)});
    //segvec.emplace_back({gkernel::Point(4, 1), gkernel::Point(6, 3)});
    //segvec.emplace_back({gkernel::Point(6, 5), gkernel::Point(9, 6)});
    //segvec.emplace_back({gkernel::Point(6, 4), gkernel::Point(9, 5)});
    //segvec.emplace_back({gkernel::Point(6, 4), gkernel::Point(8, 0)});

    segvec.emplace_back({ gkernel::Point(9, 6), gkernel::Point(10, 5) });
    segvec.emplace_back({ gkernel::Point(6, 1), gkernel::Point(10, 5) });
    segvec.emplace_back({ gkernel::Point(3.5, 5.5), gkernel::Point(4, 1) });
    segvec.emplace_back({ gkernel::Point(3.5, 5.5), gkernel::Point(6, 3) });
    segvec.emplace_back({ gkernel::Point(4, 3), gkernel::Point(6, 1) });
    segvec.emplace_back({ gkernel::Point(4, 3), gkernel::Point(6, 5) });
    segvec.emplace_back({ gkernel::Point(4, 1), gkernel::Point(6, 3) });
    segvec.emplace_back({ gkernel::Point(6, 5), gkernel::Point(9, 6) });
    segvec.emplace_back({ gkernel::Point(6, 4), gkernel::Point(9, 5) });
    segvec.emplace_back({ gkernel::Point(6, 4), gkernel::Point(8, 0) });
    segvec.emplace_back({ gkernel::Point(9, 5), gkernel::Point(11, 7) });
    segvec.emplace_back({ gkernel::Point(8, 0), gkernel::Point(13, 5) });
    segvec.emplace_back({ gkernel::Point(10, 4), gkernel::Point(13, 7) });
    segvec.emplace_back({ gkernel::Point(11, 7), gkernel::Point(13, 5) });
    segvec.emplace_back({ gkernel::Point(10, 4), gkernel::Point(12, 2) });
    segvec.emplace_back({ gkernel::Point(11, 2), gkernel::Point(13, 4) });
    segvec.emplace_back({ gkernel::Point(11, 2), gkernel::Point(12, 1) });
    segvec.emplace_back({ gkernel::Point(12, 1), gkernel::Point(15, 2) });
    segvec.emplace_back({ gkernel::Point(12, 2), gkernel::Point(16, 4) });
    segvec.emplace_back({ gkernel::Point(13, 4), gkernel::Point(15, 2) });
    segvec.emplace_back({ gkernel::Point(13, 7), gkernel::Point(14, 5) });
    segvec.emplace_back({ gkernel::Point(14, 5), gkernel::Point(16, 4) });

    RBtree res = gkernel::solve(segvec);
}

TEST_CASE("Test 1") {
    test_1();
}
