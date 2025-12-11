#pragma once
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace competitive::internal {
template <typename T, typename Cont> struct container_adaptor {};
template <typename T0, typename T1, typename Cont>
struct container_adaptor<std::pair<T0, T1>, Cont> {
  void push(common::argument<T0> v0, common::argument<T1> v1) {
    static_cast<Cont*>(this)->push(
        std::pair(std::move(v0.value), std::move(v1.value)));
  }
};
template <typename Cont, typename... Ts>
struct container_adaptor<std::tuple<Ts...>, Cont> {
  void push(common::argument<Ts>... args) {
    static_cast<Cont*>(this)->push(std::tuple<Ts...>(std::move(args.value)...));
  }
};
} // namespace competitive::internal
