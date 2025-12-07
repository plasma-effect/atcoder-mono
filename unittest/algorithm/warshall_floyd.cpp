#include "competitive/algorithm/warshall_floyd.hpp"
#include "competitive/container/dual_array.hpp"
#include <gtest/gtest.h>

TEST(WarshallFloyd, LocalUpdate) {
  {
    std::optional<int> result, a = 1, b = 2;
    competitive::internal::local_update(result, a, b);
    EXPECT_EQ(result, 3);
  }
  {
    std::optional<int> result = 4, a = 1, b = 2;
    competitive::internal::local_update(result, a, b);
    EXPECT_EQ(result, 3);
  }
  {
    std::optional<int> result = 2, a = 1, b = 2;
    competitive::internal::local_update(result, a, b);
    EXPECT_EQ(result, 2);
  }
  {
    std::optional<int> result, a, b = 2;
    competitive::internal::local_update(result, a, b);
    EXPECT_EQ(result, std::nullopt);
  }
  {
    std::optional<int> result = 4, a, b = 2;
    competitive::internal::local_update(result, a, b);
    EXPECT_EQ(result, 4);
  }
  {
    std::optional<int> result, a = 1, b;
    competitive::internal::local_update(result, a, b);
    EXPECT_EQ(result, std::nullopt);
  }
  {
    std::optional<int> result = 4, a = 1, b;
    competitive::internal::local_update(result, a, b);
    EXPECT_EQ(result, 4);
  }
  {
    int result = 4, a = 1, b = 2;
    competitive::internal::local_update(result, a, b);
    EXPECT_EQ(result, 3);
  }
}

TEST(WarshallFloyd, Execute) {
  common::dual_array<std::optional<int>> data(4, 4);
  // structure
  // 0 --> 1
  // | ┌   |
  // v   ＼v
  // 2 --> 3
  data(0, 1) = 0b0'0001;
  data(1, 3) = 0b0'0010;
  data(0, 2) = 0b0'0100;
  data(2, 3) = 0b0'1000;
  data(3, 0) = 0b1'0000;
  competitive::warshall_floyd(data);
  EXPECT_EQ(data(0, 1), 0b0'0001);
  EXPECT_EQ(data(0, 2), 0b0'0100);
  EXPECT_EQ(data(0, 3), 0b0'0011);
  EXPECT_EQ(data(1, 2), 0b1'0110);
  EXPECT_EQ(data(1, 3), 0b0'0010);
  EXPECT_EQ(data(2, 3), 0b0'1000);
}
