#include "competitive/math/monoid.hpp"
#include <gtest/gtest.h>

TEST(Math, MonoidPlus) {
  constexpr auto plus = competitive::plus<int>;
  EXPECT_EQ(plus(0b0101'0101'0101'0101, 0b1010'1010'1010'1010), 0xFFFF);
  EXPECT_EQ(plus(1, plus(2, 3)), 6);
  EXPECT_EQ(plus(), 0);
}

TEST(Math, MonoidMultiplies) {
  constexpr auto multiplies = competitive::multiplies<int>;
  EXPECT_EQ(multiplies(0b100, 0b1000), 0b100000);
  EXPECT_EQ(multiplies(2, multiplies(3, 4)), 24);
  EXPECT_EQ(multiplies(), 1);
}

TEST(Math, MonoidMin) {
  constexpr auto min = competitive::min<int>;
  EXPECT_EQ(min(10, 5), 5);
  EXPECT_EQ(min(10, min(5, 12)), 5);
  EXPECT_EQ(min(), INT_MAX);
}

TEST(Math, MonoidMax) {
  constexpr auto max = competitive::max<int>;
  EXPECT_EQ(max(10, 5), 10);
  EXPECT_EQ(max(10, max(5, 12)), 12);
  EXPECT_EQ(max(), INT_MIN);
}
