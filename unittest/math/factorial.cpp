#include "competitive/math/factorial.hpp"
#include "atcoder/all"
#include <gtest/gtest.h>

TEST(Math, FactorialInt) {
  auto fact = common::make_factorial_array<int>(5);
  ASSERT_EQ(fact.size(), 6);
  EXPECT_EQ(fact[0], 1);
  EXPECT_EQ(fact[1], 1);
  EXPECT_EQ(fact[2], 2);
  EXPECT_EQ(fact[3], 6);
  EXPECT_EQ(fact[4], 24);
  EXPECT_EQ(fact[5], 120);
}

TEST(Math, FactorialModInt) {
  using modint = atcoder::static_modint<7>;
  auto fact = common::make_factorial_array<modint>(5);
  ASSERT_EQ(fact.size(), 6);
  EXPECT_EQ(fact[0], 1);
  EXPECT_EQ(fact[1], 1);
  EXPECT_EQ(fact[2], 2);
  EXPECT_EQ(fact[3], 6);
  EXPECT_EQ(fact[4], 3);
  EXPECT_EQ(fact[5], 1);
}
