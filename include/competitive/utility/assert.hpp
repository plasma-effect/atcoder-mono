#pragma once
#include "competitive/io/debug_print.hpp"
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
#define CL_ASSERT(...)                                                         \
  (static_cast<bool>(__VA_ARGS__)                                              \
       ? void(0)                                                               \
       : common::debug::internal::fail(#__VA_ARGS__,                           \
                                       std::source_location::current()))
template <typename T>
[[noreturn]] constexpr void fail(const char* expr, const char* name,
                                 T const& value,
                                 std::source_location error_location) {
  using common::debug::println;
  println("assertion failed:", std::quoted(expr));
  println(" ", name, "=", value);
  std::cerr << "where: ";
  output_location(error_location);
  std::cerr << " from: ";
  output_location(FROM);
  std::cerr << std::flush;
  std::abort();
}
template <typename T0, typename T1>
[[noreturn]] constexpr void
fail(const char* expr, const char* name0, T0 const& value0, const char* name1,
     T1 const& value1, std::source_location error_location) {
  using common::debug::println;
  println("assertion failed:", std::quoted(expr));
  println(" ", name0, "=", value0);
  println(" ", name1, "=", value1);
  std::cerr << "where: ";
  output_location(error_location);
  std::cerr << " from: ";
  output_location(FROM);
  std::cerr << std::flush;
  std::abort();
}
#define CL_VALUE_EXPECT_1(value, ...)                                          \
  (static_cast<bool>(__VA_ARGS__)                                              \
       ? void(0)                                                               \
       : common::debug::internal::fail(#__VA_ARGS__, #value, value,            \
                                       std::source_location::current()))
#define CL_VALUE_EXPECT_2(value0, value1, ...)                                 \
  (static_cast<bool>(__VA_ARGS__)                                              \
       ? void(0)                                                               \
       : common::debug::internal::fail(#__VA_ARGS__, #value0, value0, #value1, \
                                       value1,                                 \
                                       std::source_location::current()))

#else
struct location {
  constexpr location(std::source_location) {}
  ~location() = default;
  location(location const&) = delete;
  location(location&&) = delete;
};
#define CL_ASSERT(...)                                                         \
  assert(__VA_ARGS__);                                                         \
  [[assume((__VA_ARGS__))]]
#define CL_VALUE_EXPECT_1(value, ...)                                          \
  assert(__VA_ARGS__);                                                         \
  [[assume((__VA_ARGS__))]]
#define CL_VALUE_EXPECT_2(value0, value1, ...)                                 \
  assert(__VA_ARGS__);                                                         \
  [[assume((__VA_ARGS__))]]
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
