#pragma once
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace competitive::internal {
#define CONTAINER_ADAPTOR_DEFINE(name, func)                                   \
  template <typename T, typename Cont> struct name {                           \
    void func(common::argument<T> p) {                                         \
      static_cast<Cont*>(this)->func##_i(std::move(p.value));                  \
    }                                                                          \
  };                                                                           \
  template <typename T0, typename T1, typename Cont>                           \
  struct name<std::pair<T0, T1>, Cont> {                                       \
    void func(common::argument<T0> v0, common::argument<T1> v1) {              \
      static_cast<Cont*>(this)->func##_i(                                      \
          std::pair(std::move(v0.value), std::move(v1.value)));                \
    }                                                                          \
    void func(common::argument<std::pair<T0, T1>> p) {                         \
      static_cast<Cont*>(this)->func##_i(std::move(p.value));                  \
    }                                                                          \
  };                                                                           \
  template <typename Cont, typename... Ts>                                     \
  struct name<std::tuple<Ts...>, Cont> {                                       \
    void func(common::argument<Ts>... args) {                                  \
      static_cast<Cont*>(this)->func##_i(                                      \
          std::tuple<Ts...>(std::move(args.value)...));                        \
    }                                                                          \
    void func(common::argument<std::tuple<Ts...>> t) {                         \
      static_cast<Cont*>(this)->func##_i(std::move(t.value));                  \
    }                                                                          \
  };
CONTAINER_ADAPTOR_DEFINE(push_impl, push);
CONTAINER_ADAPTOR_DEFINE(push_front_impl, push_front);
CONTAINER_ADAPTOR_DEFINE(push_back_impl, push_back);
} // namespace competitive::internal
