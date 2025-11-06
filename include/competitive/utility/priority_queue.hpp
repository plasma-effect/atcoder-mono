#pragma once
#include <bits/stdc++.h>

namespace common {
template <typename T, typename Comp = std::greater<>>
using priority_queue = std::priority_queue<T, std::vector<T>, Comp>;
}
