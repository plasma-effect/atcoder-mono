#pragma once
#include "competitive/container/internal/adaptor.hpp"
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace competitive {
template <typename T, std::size_t Cap>
class static_stack
    : public internal::container_adaptor<T, static_stack<T, Cap>> {
  std::array<T, Cap> elems_;
  std::size_t size_ = 0;

public:
  static_stack() = default;

  void push_i(T&& v) {
    CL_ASSERT(size_ < Cap);
    elems_[size_++] = std::move(v);
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
} // namespace competitive
