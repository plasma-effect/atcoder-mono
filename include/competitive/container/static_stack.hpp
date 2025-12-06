#pragma once
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace competitive {
namespace internal {
template <typename T, std::size_t Cap> class static_stack_base {
  std::array<T, Cap> elems_;
  std::size_t size_ = 0;

protected:
  template <typename... Args> void push_i(Args&&... args) {
    CL_ASSERT(size_ < Cap);
    elems_[size_++] = T(std::forward<Args>(args)...);
  }

public:
  static_stack_base() = default;
  void push(T const& v, CL_FROM_LOCATION) {
    push_i(v);
  }
  void push(T&& v, CL_FROM_LOCATION) {
    push_i(std::move(v));
  }
  T pop(CL_FROM_LOCATION) {
    CL_ASSERT(size_ > 0);
    return std::move(elems_[--size_]);
  }
  std::size_t size() const {
    return size_;
  }
  void clear() {
    size_ = 0;
  }
};
} // namespace internal
template <typename T, std::size_t Cap>
class static_stack : public internal::static_stack_base<T, Cap> {};
template <typename T0, typename T1, std::size_t Cap>
class static_stack<std::pair<T0, T1>, Cap>
    : public internal::static_stack_base<std::pair<T0, T1>, Cap> {
public:
  void push(common::argument<T0> v0, common::argument<T1> v1) {
    this->push_i(std::move(v0.value), std::move(v1.value));
  }
};
template <typename... Ts, std::size_t Cap>
class static_stack<std::tuple<Ts...>, Cap>
    : public internal::static_stack_base<std::tuple<Ts...>, Cap> {
public:
  void push(common::argument<Ts>... args) {
    this->push_i(std::move(args.value)...);
  }
};

} // namespace competitive
