#pragma once
#include "competitive/utility/assert.hpp"

namespace common {
template <std::integral Int> auto irange(Int first, Int last, FROM_LOCATION) {
  CL_ASSERT(std::cmp_less_equal(first, last));
  return std::views::iota(first, last);
}
template <std::integral Int> auto irange(Int last, FROM_LOCATION) {
  return irange(Int(0), last, _from);
}

namespace internal {
template <std::integral Int> struct dual_irange_t {
  Int f0, f1, e0, e1;
  struct iterator {
    using difference_type = Int;
    using value_type = std::pair<Int, Int>;
    using iterator_concept = std::input_iterator_tag;
    Int f0, f1, e0, e1;
    Int v0, v1;
    auto operator*() const {
      return std::pair(v0, v1);
    }
    auto& operator++() {
      if (++v1 == e1) {
        v1 = f1;
        ++v0;
      }
      return *this;
    }
    auto operator++(int) {
      auto ret = *this;
      if (++v1 == e1) {
        v1 = f1;
        ++v0;
      }
      return ret;
    }

    bool operator==(std::default_sentinel_t) const {
      return v0 == e0 || v1 == e1;
    }
  };
  auto begin() const {
    return iterator{f0, f1, e0, e1, f0, f1};
  }
  auto end() const {
    return std::default_sentinel;
  }
};
} // namespace internal
template <std::integral Int>
auto dual_irange(Int f0, Int e0, Int f1, Int e1, FROM_LOCATION) {
  CL_ASSERT(std::cmp_less_equal(f0, e0));
  CL_ASSERT(std::cmp_less_equal(f1, e1));
  return internal::dual_irange_t{f0, f1, e0, e1};
}
template <std::integral Int> auto dual_irange(Int e0, Int e1, FROM_LOCATION) {
  return dual_irange(Int(0), e0, Int(0), e1, _from);
}
} // namespace common
