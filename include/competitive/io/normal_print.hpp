#pragma once
#include "competitive/io/print_base.hpp"
#include <bits/stdc++.h>

namespace common {
inline void println() {
  std::cout << "\n";
}
template <typename... Ts> void println(Ts const&... args) {
  const char* dec[] = {"", "", " "};
  internal::print_base_t pb(std::cout, dec, dec);
  internal::print<false>(pb, args...);
  std::cout << "\n";
}
} // namespace common
