#pragma once
#include "competitive/math/monoid.hpp"
#include "competitive/utility.hpp"

namespace competitive {
namespace internal {
consteval int bit_ceil(int n) {
  return std::bit_ceil(static_cast<unsigned>(n));
}
consteval int countr_zero(int n) {
  return std::countr_zero(static_cast<unsigned>(n));
}
} // namespace internal
template <auto monoid, int N> class static_segtree {
  static_assert(N > 0);
  using value_t = internal::value_t<monoid>;
  static constexpr int size = internal::bit_ceil(N);
  static constexpr int log = internal::countr_zero(size);
  std::array<value_t, 2 * size> elems_;
  void update(int k) {
    elems_[k] = monoid(elems_[2 * k], elems_[2 * k + 1]);
  }

public:
  constexpr static_segtree() : elems_{} {
    for (auto& e : elems_) {
      e = monoid();
    }
  }
  void set(int p, value_t x, CL_FROM_LOCATION) {
    CL_VALUE_EXPECT_1(p, 0 <= p && p < N);
    p += size;
    elems_[p] = x;
    for (int i = 1; i <= log; ++i) {
      update(p >> i);
    }
  }
  auto get(int p, CL_FROM_LOCATION) const {
    CL_VALUE_EXPECT_1(p, 0 <= p && p < N);
    return elems_[p + size];
  }
  auto prod(int l, int r, CL_FROM_LOCATION) const {
    CL_VALUE_EXPECT_2(l, r, 0 <= l && l <= r && r <= N);
    auto sml = monoid(), smr = monoid();
    l += size;
    r += size;
    while (l < r) {
      if (l & 1) {
        sml = monoid(sml, elems_[l++]);
      }
      if (r & 1) {
        smr = monoid(elems_[--r], smr);
      }
      l >>= 1;
      r >>= 1;
    }
    return monoid(sml, smr);
  }
  auto all_prod() const {
    return elems_[1];
  }
};
} // namespace competitive
