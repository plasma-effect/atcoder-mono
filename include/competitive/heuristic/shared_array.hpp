#pragma once
#include "competitive/data_structure/static_segtree.hpp"
#include "competitive/utility.hpp"

namespace heuristic {
template <typename T, int N, std::size_t MaxCount = 64> class shared_array {
  static_assert(std::popcount(MaxCount) == 1, "MaxCount must be power of 2.");
  static constexpr int log = std::countr_zero(MaxCount);

  static inline std::array<T, N * MaxCount> inside_ = {};
  static inline std::array<int, 2 * MaxCount> alive_ = {};
  static inline std::array<int, MaxCount> count_ = {};

  template <bool increase> static void update(int p) {
    for (int idx = MaxCount + p; idx > 0; idx >>= 1) {
      if constexpr (increase) {
        ++alive_[idx];
      } else {
        --alive_[idx];
      }
    }
  }
  static int search() {
    int idx = 1;
    CL_ASSERT(alive_[1] != MaxCount);
    for (int i = 0, max = MaxCount; i < log; ++i) {
      idx <<= 1;
      max >>= 1;
      if (alive_[idx] == max) {
        ++idx;
      }
    }
    return idx & (MaxCount - 1);
  }

  int idx_;

public:
  shared_array(CL_FROM_LOCATION) : shared_array({}) {}
  shared_array(std::initializer_list<T> list, CL_FROM_LOCATION)
      : idx_(search()) {
    update<true>(idx_);
    ++count_[idx_];
    auto ptr = inside_.data() + N * idx_;
    int i = 0;
    for (auto&& v : list) {
      ptr[i++] = std::move(v);
    }
    for (; i < N; ++i) {
      ptr[i] = T();
    }
  }
  shared_array(shared_array const& rhs) : idx_(rhs.idx_) {
    ++count_[idx_];
  }
  shared_array& operator=(shared_array const& rhs) {
    if (--count_[idx_] == 0) {
      update<false>(idx_);
    }
    idx_ = rhs.idx_;
    ++count_[idx_];
    return *this;
  }
  ~shared_array() {
    if (--count_[idx_] == 0) {
      update<false>(idx_);
    }
  }

  T& operator[](common::argument<int> i_) {
    int i = i_.value;
    CL_VALUE_EXPECT_1(i, 0 <= i && i < N);
    return inside_[i + N * idx_];
  }
  T const& operator[](common::argument<int> i_) const {
    int i = i_.value;
    CL_VALUE_EXPECT_1(i, 0 <= i && i < N);
    return inside_[i + N * idx_];
  }
  T* data() {
    return inside_.data() + N * idx_;
  }
  T* begin() {
    return inside_.data() + N * idx_;
  }
  T const* begin() const {
    return inside_.data() + N * idx_;
  }
  T* end() {
    return inside_.data() + N * (idx_ + 1);
  }
  T const* end() const {
    return inside_.data() + N * (idx_ + 1);
  }
  auto rbegin() {
    return std::make_reverse_iterator(end());
  }
  auto rbegin() const {
    return std::make_reverse_iterator(end());
  }
  auto rend() {
    return std::make_reverse_iterator(begin());
  }
  auto rend() const {
    return std::make_reverse_iterator(begin());
  }
  int size() const {
    return N;
  }
  static int alive_count() {
    return alive_[1];
  }
  using reference = T&;
  using const_reference = T const&;
  using iterator = T*;
  using const_iterator = T const*;
  using reverse_iterator = std::reverse_iterator<T*>;
  using size_type = int;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using const_pointer = const T*;
  using value_type = T;
};
} // namespace heuristic
