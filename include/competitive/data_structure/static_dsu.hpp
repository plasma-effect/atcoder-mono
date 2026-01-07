#pragma once
#include "competitive/utility/assert.hpp"

namespace competitive {
template <int N> class static_dsu {
  std::array<int, N> parent_or_size;

public:
  static_dsu() {
    std::ranges::fill(parent_or_size, -1);
  }

  int merge(int a, int b, CL_FROM_LOCATION) {
    CL_VALUE_EXPECT_1(a, 0 <= a && a < N);
    CL_VALUE_EXPECT_1(b, 0 <= b && b < N);
    int x = leader(a), y = leader(b);
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
  bool same(int a, int b, CL_FROM_LOCATION) {
    CL_VALUE_EXPECT_1(a, 0 <= a && a < N);
    CL_VALUE_EXPECT_1(b, 0 <= b && b < N);
    return leader(a) == leader(b);
  }
  int leader(int a, CL_FROM_LOCATION) {
    CL_VALUE_EXPECT_1(a, 0 <= a && a < N);
    if (parent_or_size[a] < 0) {
      return a;
    } else {
      return parent_or_size[a] = leader(parent_or_size[a]);
    }
  }
  int size(int a, CL_FROM_LOCATION) {
    CL_VALUE_EXPECT_1(a, 0 <= a && a < N);
    return -parent_or_size[leader(a)];
  }
};
} // namespace competitive
