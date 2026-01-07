#pragma once
#include "competitive/utility.hpp"
#include <bits/stdc++.h>

namespace competitive {
namespace internal {
inline std::int64_t make_base() {
  std::uniform_int_distribution<std::int64_t> dist(2, 2LL << 50);
  std::random_device device{};
  return dist(device);
}
template <typename T>
inline std::vector<T> make_pow(T base, T mod, std::size_t N) {
  std::vector<T> ret(N + 1);
  ret[0] = 1;
  for (auto i : common::irange(N)) {
    ret[i + 1] = (ret[i] * base) % mod;
  }
  return ret;
}
} // namespace internal
template <char Min, char Max, std::size_t MaxSize = 100'000>
class rolling_hash {
  using int_t = __int128_t;
  static int_t base() {
    static const int_t b = internal::make_base();
    return b;
  }
  static int_t mod() {
    return (1LL << 61) - 1;
  }
  static const std::vector<int_t>& get_pow() {
    static const auto pow = internal::make_pow(base(), mod(), MaxSize);
    return pow;
  }
  std::vector<int_t> hashes;
  std::size_t size_;

public:
  template <std::ranges::input_range Rng>
  rolling_hash(Rng&& rng, CL_FROM_LOCATION) : hashes(), size_(std::size(rng)) {
    CL_ASSERT(size_ <= MaxSize);
    int_t hash = 0;
    hashes.reserve(size_ + 1);
    hashes.emplace_back(hash);
    for (auto c : rng) {
      CL_VALUE_EXPECT_1(c, Min <= c && c <= Max);
      hash *= base();
      hash += c - Min + 1;
      hash %= mod();
      hashes.emplace_back(hash);
    }
  }
  template <std::size_t N>
  rolling_hash(const char (&str)[N]) : rolling_hash(std::string_view(str)) {}
  rolling_hash(const rolling_hash&) = delete;
  rolling_hash& operator=(const rolling_hash&) = delete;
  rolling_hash(rolling_hash&& h)
      : hashes(std::move(h.hashes)), size_(h.size_) {}
  rolling_hash& operator=(rolling_hash&& h) {
    hashes = std::move(h.hashes);
    size_ = h.size_;
    return *this;
  }
  class subhash_t {
    const rolling_hash& parent;
    std::size_t first, len;

  public:
    subhash_t(const rolling_hash& p, std::size_t f, std::size_t s)
        : parent(p), first(f), len(s) {}
    int_t hash() const {
      const auto p = rolling_hash::get_pow()[len];
      const auto& hashes = parent.hashes;
      auto h = (hashes[first + len] - hashes[first] * p) % mod();
      return h >= 0 ? h : h + mod();
    }
    friend class rolling_hash;
  };
  subhash_t subhash(std::size_t first,
                    std::size_t len = common::max_v<std::size_t>,
                    CL_FROM_LOCATION) const {
    CL_VALUE_EXPECT_1(first, first <= size_);
    len = std::min(len, size_ - first);
    return subhash_t{*this, first, len};
  }
  auto size() const {
    return size_;
  }
  auto raw() const {
    return static_cast<std::size_t>(hashes.back());
  }
  int_t hash() const {
    return hashes.back();
  }
};
template <typename T>
concept rolling_hash_t = requires(T const& h) { h.hash(); };
template <rolling_hash_t Lhs, rolling_hash_t Rhs>
bool operator==(Lhs const& lhs, Rhs const& rhs) {
  return lhs.hash() == rhs.hash();
}
template <rolling_hash_t Lhs, rolling_hash_t Rhs>
auto operator<=>(Lhs const& lhs, Rhs const& rhs) {
  return lhs == rhs ? std::strong_ordering::equal : lhs.hash() <=> rhs.hash();
}
} // namespace competitive
namespace std {
template <char Min, char Max, std::size_t Size>
struct hash<competitive::rolling_hash<Min, Max, Size>> {
  auto operator()(competitive::rolling_hash<Min, Max, Size> const& h) const {
    return h.raw();
  }
};
} // namespace std
