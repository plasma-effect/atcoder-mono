#pragma once
#include "competitive/utility.hpp"
#include <bits/stdc++.h>

namespace competitive {
template <typename T> class coordinate_compression {
  std::vector<T> values_;

public:
  template <typename Rng> coordinate_compression(Rng&& rng) : values_() {
    values_.reserve(std::size(rng));
    for (auto&& v : rng) {
      values_.emplace_back(v);
    }
    std::ranges::sort(values_);
    auto r = std::ranges::unique(values_);
    values_.erase(r.begin(), r.end());
  }
  int size() const {
    return static_cast<int>(values_.size());
  }
  int get_index(common::argument<T const&> v) const {
    auto it = std::ranges::lower_bound(values_, v.value);
    CL_ASSERT(*it == v.value);
    return static_cast<int>(it - values_.begin());
  }
  T const& get_value(common::argument<int> i) const {
    CL_ASSERT(0 <= i.value && i.value < size());
    return values_[i.value];
  }
};
template <std::ranges::input_range Rng>
coordinate_compression(Rng&&)
    -> coordinate_compression<std::ranges::range_value_t<Rng>>;
} // namespace competitive
