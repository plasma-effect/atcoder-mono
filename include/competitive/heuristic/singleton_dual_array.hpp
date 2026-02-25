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
  T& operator()(common::argument<int> i0, common::argument<int> i1) {
    auto i0_ = i0.value;
    auto i1_ = i1.value;
    CL_VALUE_EXPECT_1(i0_, 0 <= i0_ && i0_ < H);
    CL_VALUE_EXPECT_1(i1_, 0 <= i1_ && i1_ < W);
    const auto idx = i0_ * W + i1_;
    if (version_[idx] != current_) {
      inside_[idx] = T();
      version_[idx] = current_;
    }
    return inside_[idx];
  }
};
} // namespace heuristic
