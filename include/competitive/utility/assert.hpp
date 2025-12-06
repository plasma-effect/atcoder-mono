#pragma once
#include <bits/stdc++.h>

namespace common::debug::internal {
struct location {
#ifdef LOCAL_DEBUG
  location(std::source_location);
  ~location();
#else
  location(std::source_location) {}
  ~location() = default;
#endif
  location(location const&) = delete;
  location(location&&) = delete;
};
#ifdef LOCAL_DEBUG
#define CL_ASSERT(expr)                                                        \
  (static_cast<bool>(expr) ? void(0)                                           \
                           : common::debug::internal::fail(                    \
                                 #expr, std::source_location::current()))
[[noreturn]] void fail(const char* expr, std::source_location error_location);
#else
#define CL_ASSERT(expr) assert(expr)
#endif
} // namespace common::debug::internal
#define CL_FROM_LOCATION                                                       \
  [[maybe_unused]] common::debug::internal::location _from =                   \
      std::source_location::current()
#define DEFAULT_LOCATION std::source_location()
namespace common {
template <typename T> struct argument {
  T value;
  common::debug::internal::location location;
  template <typename U>
  argument(U&& u, std::source_location loc = std::source_location::current())
      : value(std::forward<U>(u)), location(loc) {}
};
} // namespace common
