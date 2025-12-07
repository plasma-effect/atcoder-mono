#pragma once
#include "competitive/math/factorial.hpp"
#include "competitive/math/monoid.hpp"
#include "competitive/utility/assert.hpp"

namespace common {
template <std::integral T> constexpr T div_ceil(T a, T b) {
  CL_ASSERT(std::cmp_greater(b, 0));
  if (std::cmp_less(a, 0)) {
    return a / b;
  } else {
    return (a + b - 1) / b;
  }
}
} // namespace common
