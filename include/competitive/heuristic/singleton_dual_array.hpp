#pragma once
#include "competitive/utility.hpp"
#include <bits/stdc++.h>
#ifdef LOCAL_DEBUG
#include <boost/core/typeinfo.hpp>
#endif

namespace heuristic {
template <typename T, int H, int W, int id = -1> class singleton_dual_array {
  static inline std::array<T, W * H> inside_;
  static inline std::array<int, W * H> version_;
  static inline int current_;
#ifdef LOCAL_DEBUG
  static inline std::optional<std::source_location> defined_;
#endif

public:
  singleton_dual_array(CL_FROM_LOCATION) {
    ++current_;
#ifdef LOCAL_DEBUG
    if (auto prev =
            std::exchange(defined_, common::debug::internal::enter_location());
        prev) {
      boost::core::typeinfo const& ti = typeid(singleton_dual_array);
      std::cerr << std::quoted(boost::core::demangled_name(ti)) << " is alive."
                << "\n";
      std::cerr << "first definition: line = " << prev->line() << std::endl;
      std::abort();
    }
#endif
  }
#ifdef LOCAL_DEBUG
  ~singleton_dual_array() {
    defined_.reset();
  }
#endif
  void set(int i0, int i1, common::argument<T> v) {
    CL_VALUE_EXPECT_1(i0, 0 <= i0 && i0 < H);
    CL_VALUE_EXPECT_1(i1, 0 <= i1 && i1 < W);
    inside_[i0 * W + i1] = std::move(v.value);
    version_[i0 * W + i1] = current_;
  }
  T const& get(int i0, int i1, CL_FROM_LOCATION) {
    CL_VALUE_EXPECT_1(i0, 0 <= i0 && i0 < H);
    CL_VALUE_EXPECT_1(i1, 0 <= i1 && i1 < W);
    if (version_[i0 * W + i1] != current_) {
      inside_[i0 * W + i1] = T();
      version_[i0 * W + i1] = current_;
    }
    return inside_[i0 * W + i1];
  }
};
} // namespace heuristic
