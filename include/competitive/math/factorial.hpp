#pragma once
#include <bits/stdc++.h>

namespace common {
template <typename T> std::vector<T> make_factorial_array(std::size_t max) {
  std::vector<T> ret(max + 1);
  ret[0] = T(1);
  for (auto i = 0uz; i < max; ++i) {
    ret[i + 1] = ret[i] * T(i + 1);
  }
  return ret;
}
} // namespace common
