#pragma once
#include "competitive/utility/assert.hpp"
#include "competitive/utility/irange.hpp"
#include "competitive/utility/priority_queue.hpp"
#include <bits/stdc++.h>

namespace common {
template <typename T> constexpr auto max_v = std::numeric_limits<T>::max();
template <typename T> constexpr auto min_v = std::numeric_limits<T>::min();
template <typename T> using pair = std::pair<T, T>;
} // namespace common
#define IGNORE_CAT_I2(a, b) a##b
#define IGNORE_CAT_I(a, b) IGNORE_CAT_I2(a, b)
#define IGNORE [[maybe_unused]] auto IGNORE_CAT_I(_ignore, __COUNTER__)
