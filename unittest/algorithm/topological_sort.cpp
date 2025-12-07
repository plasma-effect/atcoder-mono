#include "competitive/algorithm/topological_sort.hpp"
#include "test_utils.hpp"
#include <gtest/gtest.h>

TEST(TopologicalSort, ExecuteSizeT) {
  const int N = 5;
  std::vector<std::set<std::size_t>> edge(N);
  edge[0].emplace(3);
  edge[0].emplace(4);
  edge[1].emplace(2);
  edge[1].emplace(4);
  edge[2].emplace(3);
  edge[2].emplace(4);
  auto result = competitive::topological_sort(edge);
  EXPECT_TRUE(result);
  const auto vec = *result;
  for (std::size_t i : common::irange(N)) {
    for (auto n : edge[i]) {
      auto a = std::ranges::find(vec, i);
      auto b = std::ranges::find(vec, n);
      EXPECT_LT(a, b) << "(i, n) = (" << i << ", " << n << ")";
    }
  }
}

TEST(TopologicalSort, ExecuteInt) {
  const int N = 5;
  std::vector<std::set<int>> edge(N);
  edge[0].emplace(3);
  edge[0].emplace(4);
  edge[1].emplace(2);
  edge[1].emplace(4);
  edge[2].emplace(3);
  edge[2].emplace(4);
  auto result = competitive::topological_sort(edge);
  EXPECT_TRUE(result);
  const auto vec = *result;
  for (int i : common::irange(N)) {
    for (auto n : edge[i]) {
      auto a = std::ranges::find(vec, i);
      auto b = std::ranges::find(vec, n);
      EXPECT_LT(a, b) << "(i, n) = (" << i << ", " << n << ")";
    }
  }
}

TEST(TopologicalSort, DetectCycle) {
  const int N = 3;
  std::vector<std::set<int>> edge(N);
  edge[0].emplace(1);
  edge[1].emplace(2);
  edge[2].emplace(0);
  auto result = competitive::topological_sort(edge);
  EXPECT_FALSE(result);
}
