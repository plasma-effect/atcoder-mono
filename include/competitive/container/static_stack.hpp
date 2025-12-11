#pragma once
#include "competitive/container/internal/adaptor.hpp"
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace competitive {
namespace internal {
template <typename T, std::size_t Cap> class static_stack_base {
  std::array<T, Cap> elems_;
  std::size_t size_ = 0;

public:
  static_stack_base() = default;

  void push(common::argument<T> v) {
    CL_ASSERT(size_ < Cap);
    elems_[size_++] = std::move(v.value);
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
using static_stack =
    internal::container_adaptor<T, internal::static_stack_base<T, Cap>>;
} // namespace competitive
