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
  int get_index(T const& v, CL_FROM_LOCATION) const {
    auto it = std::ranges::lower_bound(values_, v);
    CL_VALUE_EXPECT_2(*it, v, *it == v);
    return static_cast<int>(it - values_.begin());
  }
  T const& get_value(int i, CL_FROM_LOCATION) const {
    CL_VALUE_EXPECT_1(i, 0 <= i && i < size());
    return values_[i];
  }
};
template <std::ranges::input_range Rng>
coordinate_compression(Rng&&)
    -> coordinate_compression<std::ranges::range_value_t<Rng>>;
} // namespace competitive
