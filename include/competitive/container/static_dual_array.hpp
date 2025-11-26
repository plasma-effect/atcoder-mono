#pragma once
#include "competitive/io/print_base.hpp"
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace common {
template <typename T, std::size_t H, std::size_t W> class static_dual_array {
  std::array<T, W * H> inside_;

public:
  static_dual_array() : inside_{} {};
  template <std::integral Int0, std::integral Int1>
  T& operator()(Int0 i0, Int1 i1, CL_FROM_LOCATION) {
    CL_ASSERT(std::cmp_greater_equal(i0, 0) && std::cmp_less(i0, H));
    CL_ASSERT(std::cmp_greater_equal(i1, 0) && std::cmp_less(i1, W));
    return inside_[i0 * W + i1];
  }
  template <std::integral Int0, std::integral Int1>
  T const& operator()(Int0 i0, Int1 i1, CL_FROM_LOCATION) const {
    CL_ASSERT(std::cmp_greater_equal(i0, 0) && std::cmp_less(i0, H));
    CL_ASSERT(std::cmp_greater_equal(i1, 0) && std::cmp_less(i1, W));
    return inside_[i0 * W + i1];
  }
  template <std::integral Int0, std::integral Int1> T& at(Int0 i0, Int1 i1) {
    if (std::cmp_less(i0, 0) || std::cmp_greater_equal(i0, H)) [[unlikely]] {
      throw std::out_of_range(
          "argument 1 of static_dual_array::at is out of range");
    } else if (std::cmp_less(i1, 0) || std::cmp_greater_equal(i1, W))
        [[unlikely]] {
      throw std::out_of_range(
          "argument 2 of static_dual_array::at is out of range");
    }
    return inside_[i0 * W + i1];
  }
  template <std::integral Int0, std::integral Int1>
  T const& at(Int0 i0, Int1 i1) const {
    if (std::cmp_less(i0, 0) || std::cmp_greater_equal(i0, H)) [[unlikely]] {
      throw std::out_of_range(
          "argument 1 of static_dual_array::at is out of range");
    } else if (std::cmp_less(i1, 0) || std::cmp_greater_equal(i1, W))
        [[unlikely]] {
      throw std::out_of_range(
          "argument 2 of static_dual_array::at is out of range");
    }
    return inside_[i0 * W + i1];
  }

  constexpr std::pair<std::size_t, std::size_t> dimensions() const {
    return {H, W};
  }
  constexpr std::size_t size() const {
    return H * W;
  }
};
template <typename T, std::size_t H, std::size_t W>
void operator<<(internal::print_base_t& pb,
                static_dual_array<T, H, W> const& ar) {
  const auto [prefix, suffix, delim] = pb.get_range_decolater();
  const char* outer_delim = "";
  pb << prefix;
  for (auto i = 0uz; i < H; ++i) {
    pb << std::exchange(outer_delim, delim);
    const char* inner_delim = prefix;
    for (auto j = 0uz; j < W; ++j) {
      pb << std::exchange(inner_delim, delim);
      pb << ar(i, j, DEFAULT_LOCATION);
    }
    pb << suffix;
  }
  pb << suffix;
}
} // namespace common
