#pragma once
#include "competitive/container/static_queue.hpp"
#include <bits/stdc++.h>
#ifdef LOCAL_DEBUG
#include <boost/core/typeinfo.hpp>
#endif

namespace heuristic {
template <typename T, int Cap, int id = -1> class singleton_static_queue {
  static inline competitive::static_queue<T, Cap> inside_;
#ifdef LOCAL_DEBUG
  static inline std::optional<std::source_location> defined_;
#endif

public:
  singleton_static_queue(CL_FROM_LOCATION) {
    inside_.clear();
#ifdef LOCAL_DEBUG
    if (auto prev =
            std::exchange(defined_, common::debug::internal::enter_location());
        prev) {
      boost::core::typeinfo const& ti = typeid(singleton_static_queue);
      std::cerr << std::quoted(boost::core::demangled_name(ti)) << " is alive."
                << "\n";
      std::cerr << "first definition: line = " << prev->line() << std::endl;
      std::abort();
    }
#endif
  }
  void push(common::argument<T> v) {
    inside_.push(std::move(v.value));
  }
  T pop(CL_FROM_LOCATION) {
    return inside_.pop();
  }
  std::size_t size() {
    return inside_.size();
  }
};
} // namespace heuristic
