#include "competitive/algorithm/coordinate_compression.hpp"
#include <gtest/gtest.h>

TEST(CoordinateCompression, Basic) {
  std::vector coordinate = {1, 3, 5, 5, 7, 9};
  competitive::coordinate_compression compression(coordinate);
  ASSERT_EQ(compression.size(), 5);

  EXPECT_EQ(compression.get_index(1), 0);
  EXPECT_EQ(compression.get_index(3), 1);
  EXPECT_EQ(compression.get_index(5), 2);
  EXPECT_EQ(compression.get_index(7), 3);
  EXPECT_EQ(compression.get_index(9), 4);

  EXPECT_EQ(compression.get_value(0), 1);
  EXPECT_EQ(compression.get_value(1), 3);
  EXPECT_EQ(compression.get_value(2), 5);
  EXPECT_EQ(compression.get_value(3), 7);
  EXPECT_EQ(compression.get_value(4), 9);
}

TEST(CoordinateCompression, Projection) {
  std::vector<common::pair<int>> pairs = {
      {0, 1},
      {1, 3},
      {2, 5},
      {3, 7},
      {4, 9},
  };
  competitive::coordinate_compression compression(pairs | std::views::values);
  ASSERT_EQ(compression.size(), 5);

  EXPECT_EQ(compression.get_index(1), 0);
  EXPECT_EQ(compression.get_index(3), 1);
  EXPECT_EQ(compression.get_index(5), 2);
  EXPECT_EQ(compression.get_index(7), 3);
  EXPECT_EQ(compression.get_index(9), 4);

  EXPECT_EQ(compression.get_value(0), 1);
  EXPECT_EQ(compression.get_value(1), 3);
  EXPECT_EQ(compression.get_value(2), 5);
  EXPECT_EQ(compression.get_value(3), 7);
  EXPECT_EQ(compression.get_value(4), 9);
}

TEST(CoordinateCompression, OutOfBounds) {
  std::vector coordinate = {1, 3, 5, 7, 9};
  competitive::coordinate_compression compression(coordinate);

  EXPECT_DEATH({ compression.get_index(-1); }, ".");
  EXPECT_DEATH({ compression.get_index(2); }, ".");
  EXPECT_DEATH({ compression.get_index(10); }, ".");

  EXPECT_DEATH({ compression.get_value(-1); }, ".");
  EXPECT_DEATH({ compression.get_value(5); }, ".");
}
