#pragma once
#include "competitive/utility.hpp"
#include <bits/stdc++.h>

namespace competitive {
namespace internal {
template <std::integral Int> struct topological_sort_impl {
  enum class flag { none, temporary, done };
  std::vector<std::set<Int>> const& edge;
  std::vector<Int> result;
  std::vector<flag> flags;
  bool visit(Int i) {
    if (flags[i] == flag::temporary) {
      return false;
    } else if (flags[i] == flag::none) {
      flags[i] = flag::temporary;
      for (auto n : edge[i]) {
        if (!visit(n)) {
          return false;
        }
      }
      flags[i] = flag::done;
      result.emplace_back(i);
    }
    return true;
  }
};
} // namespace internal
template <std::integral Int>
std::optional<std::vector<Int>>
topological_sort(std::vector<std::set<Int>> const& edge) {
  const auto N = edge.size();
  internal::topological_sort_impl<Int> impl{edge, {}, {}};
  impl.flags.resize(N);
  for (auto i : common::irange(N)) {
    if (!impl.visit(static_cast<Int>(i))) {
      return std::nullopt;
    }
  }
  std::ranges::reverse(impl.result);
  return impl.result;
}
} // namespace competitive
