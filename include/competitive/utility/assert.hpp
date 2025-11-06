#pragma once
#include <bits/stdc++.h>

#ifdef LOCAL_DEBUG
namespace common::debug::internal {
void output_location(std::source_location loc) {
  std::cerr << loc.function_name() << ": " << loc.line() << "\n";
}
[[noreturn]] inline void assertion_fail(const char* expr,
                                        std::source_location error_location,
                                        std::source_location from_location) {
  std::cerr << "assertion failed: \"" << expr << "\"\n";
  std::cerr << "where: ";
  output_location(error_location);
  std::cerr << " from: ";
  output_location(from_location);
  std::cerr << std::flush;
  throw std::logic_error("assertion failed");
}
} // namespace common::debug::internal
#define CL_ASSERT(expr)                                                        \
  (static_cast<bool>(expr)                                                     \
       ? void(0)                                                               \
       : common::debug::internal::assertion_fail(                              \
             #expr, std::source_location::current(), _from))
#define FROM_LOCATION                                                          \
  [[maybe_unused]] std::source_location _from = std::source_location::current()
#else
#define CL_ASSERT(expr) assert(expr)
#define FROM_LOCATION [[maybe_unused]] std::nullptr_t _from = nullptr
#endif
