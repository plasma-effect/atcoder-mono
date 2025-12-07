#include "competitive/math.hpp"
#include <gtest/gtest.h>

TEST(Math, DivCeil) {
  EXPECT_EQ(common::div_ceil(4, 2), 2);
  EXPECT_EQ(common::div_ceil(5, 2), 3);
  EXPECT_EQ(common::div_ceil(-4, 2), -2);
  EXPECT_EQ(common::div_ceil(-5, 2), -2);

  EXPECT_DEATH({ common::div_ceil(2, -2); }, ".");
}
