#pragma once
#include "competitive/utility/assert.hpp"

namespace competitive {
template <int N> class static_dsu {
  std::array<int, N> parent_or_size;

public:
  static_dsu() {
    std::ranges::fill(parent_or_size, -1);
  }

  int merge(common::argument<int> a, int b) {
    CL_VALUE_EXPECT_1(a.value, 0 <= a.value && a.value < N);
    CL_VALUE_EXPECT_1(b, 0 <= b && b < N);
    int x = leader(a.value), y = leader(b);
    if (x == y) {
      return x;
    }
    if (-parent_or_size[x] < -parent_or_size[y]) {
      std::swap(x, y);
    }
    parent_or_size[x] += parent_or_size[y];
    parent_or_size[y] = x;
    return x;
  }
  bool same(common::argument<int> a, int b) {
    CL_VALUE_EXPECT_1(a.value, 0 <= a.value && a.value < N);
    CL_VALUE_EXPECT_1(b, 0 <= b && b < N);
    return leader(a.value) == leader(b);
  }
  int leader(common::argument<int> a) {
    CL_VALUE_EXPECT_1(a.value, 0 <= a.value && a.value < N);
    if (parent_or_size[a.value] < 0) {
      return a.value;
    } else {
      return parent_or_size[a.value] = leader(parent_or_size[a.value]);
    }
  }
  int size(common::argument<int> a) {
    CL_VALUE_EXPECT_1(a.value, 0 <= a.value && a.value < N);
    return -parent_or_size[leader(a.value)];
  }
};
} // namespace competitive
