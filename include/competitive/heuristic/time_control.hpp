#pragma once
#include "competitive/heuristic/random.hpp"
#include "competitive/utility/assert.hpp"
#include <bits/stdc++.h>

namespace heuristic {
using time_t = std::chrono::milliseconds;
template <std::int64_t time_limit_ms, std::size_t update_frequency = 1>
class time_control_t {
  std::size_t update_count_ = 0, total_update_count_ = 0;
  bool enable_ = true;
  std::chrono::system_clock::time_point start_;
  time_t current_;

public:
  time_control_t() : start_(std::chrono::system_clock::now()), current_() {}
  operator bool() {
    ++total_update_count_;
    if constexpr (update_frequency != 1) {
      if (++update_count_ == update_frequency) {
        update_count_ = 0;
        current_ = std::chrono::duration_cast<time_t>(
            std::chrono::system_clock::now() - start_);
        return enable_ = (current_.count() < time_limit_ms);
      } else {
        return enable_;
      }
    } else {
      current_ = std::chrono::duration_cast<time_t>(
          std::chrono::system_clock::now() - start_);
      return (current_.count() < time_limit_ms);
    }
  }
  time_t current() const {
    return current_;
  }
  std::size_t total_update_count() const {
    return total_update_count_;
  }
};

template <std::int64_t time_limit_ms, std::int64_t start_temperature,
          std::int64_t end_temperature, std::size_t update_frequency = 1,
          typename Engine = xorshift>
class time_control_with_annealing
    : public time_control_t<time_limit_ms, update_frequency> {
  static constexpr std::size_t particle = 1 << 8;
  static constexpr std::array<double, particle> make_log_table() {
    std::array<double, particle> table;
    table[0] = -std::numeric_limits<double>::infinity();
    for (std::size_t i = 1; i < particle; ++i) {
      table[i] = std::log(double(i) / double(particle));
    }
    return table;
  }
  static constexpr std::array<double, time_limit_ms> make_temperature_table() {
    std::array<double, time_limit_ms> table;
    double T1 = end_temperature;
    double dT = double(start_temperature) / double(end_temperature);
    for (int i = 0; i < time_limit_ms; ++i) {
      double nt = i / double(time_limit_ms);
      table[i] = T1 * std::pow(dT, 1 - nt);
    }
    return table;
  }
  static constexpr auto log_table = make_log_table();
  static constexpr auto temp_table = make_temperature_table();

  std::uniform_int_distribution<std::size_t> dist;
  Engine engine;

public:
  time_control_with_annealing() : dist(0uz, particle - 1), engine() {}
  bool transition_check(double diff) {
    CL_ASSERT(this->current().count() < time_limit_ms);
    if (diff > 0) {
      return true;
    } else {
      auto p = dist(engine);
      return log_table[p] * temp_table[this->current().count()] < diff;
    }
  }
};
} // namespace heuristic
