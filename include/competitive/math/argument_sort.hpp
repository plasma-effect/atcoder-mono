#pragma once
#include "competitive/utility.hpp"

namespace common {
struct argument_sort_t {
  constexpr bool operator()(const point_t& lhs, const point_t& rhs) const {
    std::int64_t x0 = lhs.first, y0 = lhs.second;
    std::int64_t x1 = rhs.first, y1 = rhs.second;
    if(x0 == 0 && y0 == 0) {
      return false;
    } else if(x1 == 0 && y1 == 0) {
      return true;
    }
    int c0 = (y0 < 0 || (y0 == 0 && x0 < 0));
    int c1 = (y1 < 0 || (y1 == 0 && x1 < 0));
    if (c0 != c1) {
      return c0 < c1;
    } else if(std::int64_t c = x0 * y1 - x1 * y0; c == 0) {
      return std::abs(x0) + std::abs(y0) < std::abs(x1) + std::abs(y1);
    } else {
      return c > 0;
    }
  }
};
constexpr argument_sort_t argument_sort{};
} // namespace common
