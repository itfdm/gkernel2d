#include "gkernel/rbtree.h"
#include "test.h"

using int_tree = gkernel::RBTree<int>;

void test_insert()
{
    int_tree tree;
    tree.insert({5, 3, 9, 6, 1});
    REQUIRE(tree.size() == 5);
    REQUIRE(tree.min() == 1);
    REQUIRE(tree.max() == 9);
}

void test_prev_next() {
    int_tree tree;
    tree.insert({5, 3, 9, 6, 1});

    REQUIRE(tree.find_prev(8).first == 6);
    REQUIRE(tree.find_prev(9).first == 6);
    REQUIRE(tree.size() == 5);

    REQUIRE(tree.find_next(7).first == 9);
    REQUIRE(tree.find_next(6).first == 9);
    REQUIRE(tree.size() == 5);

    REQUIRE(tree.find_next(10).second == int_tree::inf_state::is_positive);
    REQUIRE(tree.find_prev(0).second == int_tree::inf_state::is_negative);
    REQUIRE(tree.size() == 5);
}

TEST_CASE("Test item insertion")
{
    test_insert();
}

TEST_CASE("Test prev/next") {
    test_prev_next();
}
