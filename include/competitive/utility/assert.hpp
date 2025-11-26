#pragma once
#include <bits/stdc++.h>

#ifdef LOCAL_DEBUG
namespace common::debug::internal {
struct location {
  location(std::source_location);
  location(location const&) = delete;
  location(location&&) = delete;
  ~location();
};
[[noreturn]] void fail(const char* expr, std::source_location error_location);
} // namespace common::debug::internal

#define CL_ASSERT(expr)                                                        \
  (static_cast<bool>(expr) ? void(0)                                           \
                           : common::debug::internal::fail(                    \
                                 #expr, std::source_location::current()))
#define CL_FROM_LOCATION                                                       \
  [[maybe_unused]] common::debug::internal::location _from =                   \
      std::source_location::current()
#define DEFAULT_LOCATION std::source_location()
#else
#define CL_ASSERT(expr) assert(expr)
#define CL_FROM_LOCATION [[maybe_unused]] std::nullptr_t _from = nullptr
#define DEFAULT_LOCATION nullptr
#endif
