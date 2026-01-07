#pragma once
#include "competitive/io/print_base.hpp"
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace common {
template <typename T, int H, int W> class static_dual_array {
  std::array<T, W * H> inside_;

public:
  static_dual_array() : inside_{} {};
  T& operator()(common::argument<int> i0_, common::argument<int> i1_) {
    const auto i0 = i0_.value;
    const auto i1 = i1_.value;
    CL_VALUE_EXPECT_1(i0, 0 <= i0 && i0 < H);
    CL_VALUE_EXPECT_1(i1, 0 <= i1 && i1 < W);
    return inside_[i0 * W + i1];
  }
  T const& operator()(common::argument<int> i0_,
                      common::argument<int> i1_) const {
    const auto i0 = i0_.value;
    const auto i1 = i1_.value;
    CL_VALUE_EXPECT_1(i0, 0 <= i0 && i0 < H);
    CL_VALUE_EXPECT_1(i1, 0 <= i1 && i1 < W);
    return inside_[i0 * W + i1];
  }
  T& at(int i0, int i1) {
    if (i0 < 0 || i0 >= H) [[unlikely]] {
      throw std::out_of_range(
          "argument 1 of static_dual_array::at is out of range");
    } else if (i1 < 0 || i1 >= W) [[unlikely]] {
      throw std::out_of_range(
          "argument 2 of static_dual_array::at is out of range");
    }
    return inside_[i0 * W + i1];
  }
  T const& at(int i0, int i1) const {
    if (i0 < 0 || i0 >= H) [[unlikely]] {
      throw std::out_of_range(
          "argument 1 of static_dual_array::at is out of range");
    } else if (i1 < 0 || i1 >= W) [[unlikely]] {
      throw std::out_of_range(
          "argument 2 of static_dual_array::at is out of range");
    }
    return inside_[i0 * W + i1];
  }

  constexpr std::pair<int, int> dimensions() const {
    return {H, W};
  }
  constexpr int size() const {
    return H * W;
  }
};
template <typename T, int H, int W>
void operator<<(internal::print_base_t& pb,
                static_dual_array<T, H, W> const& ar) {
  const auto [prefix, suffix, delim] = pb.get_range_decolater();
  const char* outer_delim = "";
  pb << prefix;
  for (int i = 0; i < H; ++i) {
    pb << std::exchange(outer_delim, delim);
    const char* inner_delim = prefix;
    for (int j = 0; j < W; ++j) {
      pb << std::exchange(inner_delim, delim);
      pb << ar(i, j);
    }
    pb << suffix;
  }
  pb << suffix;
}
} // namespace common
