#include "gkernel/rbtree.hpp"
#include "test.hpp"

using int_tree = gkernel::RBTree<int>;

void test_insert()
{
    int_tree tree;
    tree.insert({5, 3, 9, 6, 1});
    REQUIRE(tree.size() == 5);
    tree.insert(5);
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

    REQUIRE(tree.find_next(555).second == int_tree::state::inf_positive);
    REQUIRE(tree.find_prev(-100).second == int_tree::state::inf_negative);
    REQUIRE(tree.size() == 5);
}

void test_erase() {
    int_tree tree;
    tree.insert({55, 2342, 123, 543, -123, 190, 500});
    REQUIRE(tree.size() == 7);
    tree.erase(55);
    REQUIRE(tree.size() == 6);
    tree.erase(55);
    REQUIRE(tree.size() == 6);
    tree.erase(-123);
    tree.erase(500);
    tree.erase(2342);
    REQUIRE(tree.size() == 3);
}

TEST_CASE("Test item insertion")
{
    test_insert();
}

TEST_CASE("Test prev/next") {
    test_prev_next();
}

TEST_CASE("Test erase") {
    test_erase();
}
