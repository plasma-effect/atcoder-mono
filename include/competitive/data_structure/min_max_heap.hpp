#pragma once
#include "boost/container/static_vector.hpp"
#include "competitive/utility.hpp"
#include <bits/stdc++.h>

namespace competitive {
template <std::movable T, typename Container = std::vector<T>>
class min_max_heap {
  Container values;
  template <bool is_less> bool compare(std::size_t i0, std::size_t i1) {
    if constexpr (is_less) {
      return values[i0 - 1] < values[i1 - 1];
    } else {
      return values[i0 - 1] > values[i1 - 1];
    }
  }

  bool is_min_level(std::size_t index) {
    CL_ASSERT(index > 0);
    return std::countl_zero(index) % 2 != 0;
  }
  template <bool is_min> void bubble_up(std::size_t index) {
    if (index >= 4 && compare<is_min>(index, index / 4)) {
      std::swap(values[index / 4 - 1], values[index - 1]);
      bubble_up<is_min>(index / 4);
    }
  }
  void bubble_up_init(std::size_t index) {
    if (index > 1) {
      auto parent = index / 2;
      if (is_min_level(index)) {
        if (compare<true>(parent, index)) {
          std::swap(values[parent - 1], values[index - 1]);
          bubble_up<false>(parent);
        } else {
          bubble_up<true>(index);
        }
      } else {
        if (compare<false>(parent, index)) {
          std::swap(values[parent - 1], values[index - 1]);
          bubble_up<true>(parent);
        } else {
          bubble_up<false>(index);
        }
      }
    }
  }
  template <bool is_min> void trickle_down(std::size_t i) {
    if (2 * i > values.size()) {
      return;
    }
    std::size_t t = 2 * i;
    for (std::size_t idx :
         {2 * i + 1, 4 * i, 4 * i + 1, 4 * i + 2, 4 * i + 3}) {
      if (idx > values.size()) {
        break;
      }
      if (compare<is_min>(idx, t)) {
        t = idx;
      }
    }
    if (compare<is_min>(t, i)) {
      std::swap(values[t - 1], values[i - 1]);
      if (t / 4 == i) {
        if (compare<is_min>(t / 2, t)) {
          std::swap(values[t / 2 - 1], values[t - 1]);
        }
        trickle_down<is_min>(t);
      }
    }
  }

public:
  min_max_heap() : values{} {}

  template <typename U> void push(U&& u, CL_FROM_LOCATION) {
    values.push_back(std::forward<U>(u));
    bubble_up_init(values.size());
  }

  T const& min(CL_FROM_LOCATION) const {
    CL_ASSERT(values.size() > 0);
    return values[0];
  }
  T const& max(CL_FROM_LOCATION) const {
    CL_ASSERT(values.size() > 0);
    if (values.size() < 3) {
      return values.back();
    } else {
      return std::max(values[1], values[2], comp);
    }
  }
  std::size_t size() const {
    return values.size();
  }

  void pop_min(CL_FROM_LOCATION) {
    CL_ASSERT(size() != 0);
    std::swap(values.front(), values.back());
    values.pop_back();
    trickle_down<true>(1);
  }
  void pop_max(CL_FROM_LOCATION) {
    CL_ASSERT(size() != 0);
    if (values.size() > 1) {
      std::size_t i = values.size() > 2 && compare<true>(2, 3) ? 3 : 2;
      std::swap(values[i - 1], values.back());
      values.pop_back();
      trickle_down<false>(i);
    } else {
      values.pop_back();
    }
  }
};
template <typename T, std::size_t Capacity>
using static_min_max_heap =
    min_max_heap<T, boost::container::static_vector<T, Capacity>>;
} // namespace competitive
