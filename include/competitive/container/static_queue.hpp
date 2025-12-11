#pragma once
#include "competitive/container/internal/adaptor.hpp"
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace competitive {
namespace internal {
template <typename T, std::size_t Cap> class static_queue_base {
  std::array<T, Cap> elems_;
  T* first_;
  T* last_;
  std::size_t size_;

public:
  static_queue_base()
      : elems_{}, first_(elems_.begin()), last_(elems_.begin()), size_() {}
  void push(common::argument<T> v) {
    CL_ASSERT(size_ < Cap);
    *last_ = std::move(v.value);
    if (++last_ == elems_.end()) {
      last_ = elems_.begin();
    }
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
} // namespace internal
template <typename T, std::size_t Cap>
using static_queue =
    internal::container_adaptor<T, internal::static_queue_base<T, Cap>>;
} // namespace competitive
