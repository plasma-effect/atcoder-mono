#pragma once
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace competitive {
namespace internal {
template <typename T, std::size_t Cap> class static_queue_base {
  std::array<T, Cap> elems_;
  T* first_;
  T* last_;
  std::size_t size_;

protected:
  template <typename... Args> void push_i(Args&&... args) {
    CL_ASSERT(size_ < Cap);
    *last_ = T(std::forward<Args>(args)...);
    if (++last_ == elems_.end()) {
      last_ = elems_.begin();
    }
    ++size_;
  }

public:
  static_queue_base()
      : elems_{}, first_(elems_.begin()), last_(elems_.begin()), size_() {}
  void push(T const& v, CL_FROM_LOCATION) {
    push_i(v);
  }
  void push(T&& v, CL_FROM_LOCATION) {
    push_i(std::move(v));
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
class static_queue : public internal::static_queue_base<T, Cap> {};
template <typename T0, typename T1, std::size_t Cap>
class static_queue<std::pair<T0, T1>, Cap>
    : public internal::static_queue_base<std::pair<T0, T1>, Cap> {
public:
  void push(common::argument<T0> v0, common::argument<T1> v1) {
    this->push_i(std::move(v0.value), std::move(v1.value));
  }
};
template <typename... Ts, std::size_t Cap>
class static_queue<std::tuple<Ts...>, Cap>
    : public internal::static_queue_base<std::tuple<Ts...>, Cap> {
public:
  void push(common::argument<Ts>... args) {
    this->push_i(std::move(args.value)...);
  }
};
} // namespace competitive
