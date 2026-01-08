#pragma once
#include "competitive/utility.hpp"
#include <bits/stdc++.h>

namespace competitive {
namespace internal {
#define DEFINE_MONOID(name, e, op)                                             \
  template <typename T> struct name {                                          \
    constexpr T operator()() const {                                           \
      return e;                                                                \
    }                                                                          \
    constexpr T operator()(T const& lhs, T const& rhs) const {                 \
      return op;                                                               \
    }                                                                          \
  }
DEFINE_MONOID(plus_t, T(), lhs + rhs);
DEFINE_MONOID(multiplies_t, T(1), lhs* rhs);
DEFINE_MONOID(min_t, common::max_v<T>, std::min(lhs, rhs));
DEFINE_MONOID(max_t, common::min_v<T>, std::max(lhs, rhs));
#undef DEFINE_MONOID
template <auto monoid> using value_t = decltype(monoid());
} // namespace internal
template <typename T> constexpr internal::plus_t<T> plus{};
template <typename T> constexpr internal::multiplies_t<T> multiplies{};
template <typename T> constexpr internal::min_t<T> min{};
template <typename T> constexpr internal::max_t<T> max{};
} // namespace competitive
