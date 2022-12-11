#ifndef __GKERNEL_HPP_TEST
#define __GKERNEL_HPP_TEST

#if !defined(DOCTEST_CONFIG_IMPLEMENT)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#endif

#include "doctest.hpp"

#define DECLARE_TEST(TestName) TEST_CASE(#TestName) { TestName(); }

#endif // __GKERNEL_HPP_TEST
