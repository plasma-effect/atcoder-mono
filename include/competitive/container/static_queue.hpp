#pragma once
#include "competitive/container/internal/adaptor.hpp"
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace competitive {
template <typename T, std::size_t Cap>
class static_queue : public internal::push_back_impl<T, static_queue<T, Cap>>,
                     public internal::push_front_impl<T, static_queue<T, Cap>> {
  std::array<T, Cap> elems_;
  T* first_;
  T* last_;
  std::size_t size_;

public:
  static_queue()
      : elems_{}, first_(elems_.begin()), last_(elems_.begin()), size_() {}
  void push_back_i(T&& v) {
    CL_ASSERT(size_ < Cap);
    *last_ = std::move(v);
    if (++last_ == elems_.end()) {
      last_ = elems_.begin();
    }
    ++size_;
  }
  void push_front_i(T&& v) {
    CL_ASSERT(size_ < Cap);
    if (first_ == elems_.begin()) {
      first_ = elems_.end();
    }
    *(--first_) = std::move(v);
    ++size_;
  }
  T pop(CL_FROM_LOCATION) {
    CL_ASSERT(size_ > 0);
    auto ret = std::move(*first_);
    if (++first_ == elems_.end()) {
      first_ = elems_.begin();
    }
    --size_;
    return ret;
  }
  std::size_t size() const {
    return size_;
  }
  void clear() {
    size_ = 0;
    first_ = last_;
  }
};
} // namespace competitive
