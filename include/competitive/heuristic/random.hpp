#pragma once
#include <bits/stdc++.h>

namespace heuristic {
struct xorshift {
  std::uint64_t state;
  xorshift(std::uint64_t seed = 1) : state(seed) {}
  std::uint64_t operator()() {
    state ^= state << 13;
    state ^= state >> 7;
    state ^= state << 17;
    return state ^ (state >> 32);
  }
  using result_type = std::uint64_t;
  static constexpr std::uint64_t min() {
    return 1;
  }
  static constexpr std::uint64_t max() {
    return std::numeric_limits<std::uint64_t>::max();
  }
};

template <typename Engine = xorshift> class random_engine_generator {
  static auto& get_engine() {
    thread_local Engine engine;
    return engine;
  }
  template <typename T> static auto make_distribution(T&& dist) {
    return [&engine = get_engine(), dist = std::move(dist)] mutable {
      return dist(engine);
    };
  }

public:
  template <typename T> static auto uniform_int_distribution(T min, T max) {
    return make_distribution(std::uniform_int_distribution<T>(min, max));
  }
  template <typename T> static auto uniform_int_distribution(T max) {
    return uniform_int_distribution(T(0), max);
  }
  template <typename T> static auto uniform_real_distribution(T min, T max) {
    return make_distribution(std::uniform_real_distribution<T>(min, max));
  }
  template <typename T> static auto uniform_real_distribution(T max) {
    return uniform_real_distribution<T>(0, max);
  }
  static auto bernoulli_distribution(double p = 0.5) {
    return make_distribution(std::bernoulli_distribution(p));
  }
  template <std::input_iterator Ite>
  static auto discrete_distribution(const Ite& first, const Ite& last) {
    return make_distribution(
        std::discrete_distribution<std::size_t>(first, last));
  }
  template <std::ranges::input_range Rng>
  static auto discrete_distribution(const Rng& rng) {
    return make_distribution(std::discrete_distribution<std::size_t>(
        std::ranges::begin(rng), std::ranges::end(rng)));
  }
  static auto discrete_distribution(std::initializer_list<double> list) {
    return discrete_distribution(list.begin(), list.end());
  }
  static double generate_canonical() {
    constexpr auto digits = std::numeric_limits<double>::digits;
    return std::generate_canonical<double, digits>(get_engine());
  }
  template <typename Rng> static void shuffle(Rng& rng) {
    std::ranges::shuffle(rng, get_engine());
  }
  template <std::random_access_iterator It>
  static void shuffle(It first, It last) {
    std::shuffle(first, last, get_engine());
  }
};
} // namespace heuristic
