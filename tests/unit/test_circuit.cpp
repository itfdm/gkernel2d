#include "gkernel/circuit.h"
#include "test.h"

void test_point_insert()
{
    gkernel::Circuit circuit(1);
    circuit.push({42, 15});
    REQUIRE(circuit[0].x == 42);
    REQUIRE(circuit[0].y == 15);
}

TEST_CASE("Test one point insertion into circuit")
{
    test_point_insert();
}
