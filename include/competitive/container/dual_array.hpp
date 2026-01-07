#pragma once
#include "competitive/io/print_base.hpp"
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace common {
template <typename T> class dual_array {
  std::vector<T> inside_;
  int dim0, dim1;

public:
  dual_array(int d0, int d1) : inside_(d0 * d1), dim0(d0), dim1(d1) {}
  T& operator()(common::argument<int> i0, common::argument<int> i1) {
    CL_VALUE_EXPECT_1(i0.value, 0 <= i0.value && i0.value < dim0);
    CL_VALUE_EXPECT_1(i1.value, 0 <= i1.value && i1.value < dim1);
    return inside_[i0.value * dim1 + i1.value];
  }
  T const& operator()(common::argument<int> i0,
                      common::argument<int> i1) const {
    CL_VALUE_EXPECT_1(i0.value, 0 <= i0.value && i0.value < dim0);
    CL_VALUE_EXPECT_1(i1.value, 0 <= i1.value && i1.value < dim1);
    return inside_[i0.value * dim1 + i1.value];
  }
  T& at(int i0, int i1) {
    if (i0 < 0 || i0 >= dim0) [[unlikely]] {
      throw std::out_of_range("argument 1 of dual_array::at is out of range");
    } else if (i1 < 0 || i1 >= dim1) [[unlikely]] {
      throw std::out_of_range("argument 2 of dual_array::at is out of range");
    }
    return inside_[i0 * dim1 + i1];
  }
  T const& at(int i0, int i1) const {
    if (i0 < 0 || i0 >= dim0) [[unlikely]] {
      throw std::out_of_range("argument 1 of dual_array::at is out of range");
    } else if (i1 < 0 || i1 >= dim1) [[unlikely]] {
      throw std::out_of_range("argument 2 of dual_array::at is out of range");
    }
    return inside_[i0 * dim1 + i1];
  }

  std::pair<int, int> dimensions() const {
    return {dim0, dim1};
  }
  int size() const {
    return dim0 * dim1;
  }
};

namespace internal {
struct bool_ploxy {
  bool v = false;
  operator bool() const {
    return v;
  }
  bool_ploxy& operator=(bool u) {
    v = u;
    return *this;
  }
};
} // namespace internal
template <> class dual_array<bool> : public dual_array<internal::bool_ploxy> {
public:
  using dual_array<internal::bool_ploxy>::dual_array;
};
template <typename T>
void operator<<(internal::print_base_t& pb, dual_array<T> const& ar) {
  auto [H, W] = ar.dimensions();
  const auto [prefix, suffix, delim] = pb.get_range_decolater();
  const char* outer_delim = prefix;
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
