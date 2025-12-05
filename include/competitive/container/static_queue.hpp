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
  template <typename U> void push_i(U&& v) {
    CL_ASSERT(size_ < Cap);
    *last_ = std::forward<U>(v);
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
  template <typename U0, typename U1>
  void push(U0&& u0, U1&& u1, CL_FROM_LOCATION) {
    this->push_i(std::pair<T0, T1>(std::forward<U0>(u0), std::forward<U1>(u1)));
  }
};
template <typename... Ts, std::size_t Cap>
class static_queue<std::tuple<Ts...>, Cap>
    : public internal::static_queue_base<std::tuple<Ts...>, Cap> {
public:
  void push(Ts const&... args, CL_FROM_LOCATION) {
    this->push_i(std::tuple<Ts...>(args...));
  }
};
} // namespace competitive
