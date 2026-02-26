#pragma once
#include "competitive/container/static_queue.hpp"
#include <bits/stdc++.h>
#ifdef LOCAL_DEBUG
#include <boost/core/typeinfo.hpp>
#endif

namespace heuristic {
template <typename T, int Cap, int id = -1>
class singleton_static_queue : public competitive::internal::push_back_impl<
                                   T, singleton_static_queue<T, Cap, id>>,
                               public competitive::internal::push_front_impl<
                                   T, singleton_static_queue<T, Cap, id>> {
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
#ifdef LOCAL_DEBUG
  ~singleton_static_queue() {
    defined_.reset();
  }
#endif
  void push_back_i(T&& v) {
    inside_.push_back(std::move(v));
  }
  void push_front_i(T&& v) {
    inside_.push_front(std::move(v));
  }
  T pop(CL_FROM_LOCATION) {
    return inside_.pop();
  }
  std::size_t size() {
    return inside_.size();
  }
};
} // namespace heuristic
