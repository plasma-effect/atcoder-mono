#include "competitive/utility/irange.hpp"
#include <gtest/gtest.h>

TEST(Utility, Irange1Arg) {
  std::vector vec = {0, 1, 2, 3, 4};
  ASSERT_TRUE(std::ranges::equal(vec, common::irange(5)));
}

TEST(Utility, Irange2Args) {
  std::vector vec = {3, 4, 5, 6, 7};
  ASSERT_TRUE(std::ranges::equal(vec, common::irange(3, 8)));
}

TEST(Utility, IrangeReverse) {
  std::vector vec = {4, 3, 2, 1, 0};
  ASSERT_TRUE(std::ranges::equal(vec, common::irange(5) | std::views::reverse));
}

TEST(Utility, IrangeLength0) {
  std::vector<int> vec;
  ASSERT_TRUE(std::ranges::equal(vec, common::irange(0)));
}

TEST(Utility, DualIrange2Args) {
  std::vector<std::pair<int, int>> vec = {
      {0, 0},
      {0, 1},
      {0, 2},
      {1, 0},
      {1, 1},
      {1, 2},
      {2, 0},
      {2, 1},
      {2, 2},
  };
  ASSERT_TRUE(std::ranges::equal(vec, common::dual_irange(3, 3)));
}

TEST(Utility, DualIrange4Args) {
  std::vector<std::pair<int, int>> vec = {
      {2, 1},
      {2, 2},
      {2, 3},
      {3, 1},
      {3, 2},
      {3, 3},
      {4, 1},
      {4, 2},
      {4, 3},
  };
  ASSERT_TRUE(std::ranges::equal(vec, common::dual_irange(2, 5, 1, 4)));
}

TEST(Utility, DualIrangeLength02) {
  std::vector<std::pair<int, int>> vec{};
  ASSERT_TRUE(std::ranges::equal(vec, common::dual_irange(0, 2)));
}

TEST(Utility, DualIrangeLength20) {
  std::vector<std::pair<int, int>> vec{};
  ASSERT_TRUE(std::ranges::equal(vec, common::dual_irange(2, 0)));
}
