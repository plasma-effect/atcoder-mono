#pragma once
#include "bits/stdc++.h"
#include "competitive/utility.hpp"

namespace competitive {
namespace internal {
common::pair<int> set_distance(int i, int c,
                               std::vector<std::set<int>> const& edges,
                               std::vector<std::optional<int>>& memo,
                               std::vector<int>& count) {
  int max = *memo[i];
  int p = i;
  for (auto n : edges[i]) {
    if (count[n] != c) {
      count[n] = c;
      memo[n] = *memo[i] + 1;
      auto [d, q] = set_distance(n, c, edges, memo, count);
      if (d > max) {
        max = d;
        p = q;
      }
    }
  }
  return {max, p};
}
} // namespace internal
int get_tree_diam(int& center, std::vector<std::set<int>> const& edges,
                  CL_FROM_LOCATION) {
  static std::vector<std::optional<int>> memo;
  static std::vector<int> count;
  static int version = 0;
  const auto N = edges.size();
  memo.resize(N);
  count.resize(N);
  ++version;
  memo[0] = 0;
  count[0] = version;
  auto [_, p0] = internal::set_distance(0, version, edges, memo, count);
  memo[p0] = 0;
  count[p0] = version++;
  auto [d, p1] = internal::set_distance(p0, version, edges, memo, count);
  for (auto i : common::irange(N)) {
    CL_ASSERT(count[i] == version && memo[i]);
    if (*memo[i] == d / 2) {
      center = static_cast<int>(i);
      break;
    }
  }
  return d;
}
} // namespace competitive
