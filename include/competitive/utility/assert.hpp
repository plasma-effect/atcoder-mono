#pragma once
#include <bits/stdc++.h>

namespace common::debug::internal {
#ifdef LOCAL_DEBUG
inline std::source_location FROM = std::source_location::current();
inline int depth = 0;
inline void output_location(std::source_location loc) {
  std::cerr << loc.function_name() << ": " << loc.line() << "\n";
}
inline std::source_location enter_location() {
  return FROM;
}
[[noreturn]] constexpr void fail(const char* expr,
                                 std::source_location error_location) {
  std::cerr << "assertion failed: \"" << expr << "\"\n";
  std::cerr << "where: ";
  output_location(error_location);
  std::cerr << " from: ";
  output_location(FROM);
  std::cerr << std::flush;
  std::abort();
}
struct location {
  int s;
  constexpr location(std::source_location loc)
      : s{std::is_constant_evaluated() ? 0 : 1} {
    if !consteval {
      if (depth++ == 0) {
        FROM = loc;
      }
    }
  }
  constexpr ~location() {
    if !consteval {
      depth -= s;
    }
  }
  location(location const&) = delete;
  location(location&&) = delete;
};
#define CL_ASSERT(expr)                                                        \
  (static_cast<bool>(expr) ? void(0)                                           \
                           : common::debug::internal::fail(                    \
                                 #expr, std::source_location::current()))
#else
struct location {
  constexpr location(std::source_location) {}
  ~location() = default;
  location(location const&) = delete;
  location(location&&) = delete;
};
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
  constexpr argument(U&& u,
                     std::source_location loc = std::source_location::current())
      : value(std::forward<U>(u)), location(loc) {}
};
} // namespace common
