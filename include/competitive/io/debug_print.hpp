#pragma once
#include "competitive/io/print_base.hpp"
#include <bits/stdc++.h>

namespace common::debug {
inline void println() {
  std::cerr << std::endl;
}
template <typename... Ts> void println(Ts const&... args) {
  const char* rng[] = {"{", "}", ", "};
  const char* tpl[] = {"(", ")", ", "};
  common::internal::print_base_t pb(std::cerr, rng, tpl);
  common::internal::print<false>(pb, args...);
  std::cerr << std::endl;
}
} // namespace common::debug
#ifdef LOCAL_DEBUG
#define CL_PP_STR_I(s) #s
#define CL_PP_STR(s) CL_PP_STR_I(s)
#define DEBUG_PRINT(...)                                                       \
  common::debug::println(CL_PP_STR(__LINE__) ":" __VA_OPT__(, ) __VA_ARGS__)
#else
#define DEBUG_PRINT(...) (void)(0)
#endif
