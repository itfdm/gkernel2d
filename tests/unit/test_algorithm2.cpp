#include "test.hpp"

#include <gkernel/algorithm2.hpp>
#include "gkernel/objects.hpp"
#include "gkernel/containers.hpp"

#include <unordered_set>
#include <algorithm>

using namespace gkernel;

void TestSegments1() {
    SegmentsSet input;
    input.emplace_back({Point(1, 2), Point(4, 6)});
    input.emplace_back({Point(4, 6), Point(5.5, 8)});
    input.emplace_back({Point(5.5, 8), Point(6, 9)});
    input.emplace_back({Point(6, 9), Point(7, 8)});
    input.emplace_back({Point(7, 8), Point(9, 5)});
    input.emplace_back({Point(9, 5), Point(11, 2)});
    input.emplace_back({Point(11, 2), Point(1, 2)});

    input.emplace_back({Point(3, 8), Point(5.5, 8)});
    input.emplace_back({Point(5.5, 8), Point(7, 8)});
    input.emplace_back({Point(7, 8), Point(12, 8)});
    input.emplace_back({Point(12, 8), Point(9, 5)});
    input.emplace_back({Point(9, 5), Point(6, 3)});
    input.emplace_back({Point(6, 3), Point(4, 6)});
    input.emplace_back({Point(4, 6), Point(3, 8)});
    
    input.set_labels_types({1});
    input.set_label_values(1, {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1});

    auto result = SegmentPair::segmentlabel(input);

}

DECLARE_TEST(TestSegments1);