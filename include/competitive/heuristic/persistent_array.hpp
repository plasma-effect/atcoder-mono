#pragma once
#include "competitive/math.hpp"
#include "competitive/utility.hpp"
#include <bits/stdc++.h>

namespace heuristic {
template <std::copy_constructible T, std::size_t N> class persistent_array {
  static constexpr std::size_t total_size = 512;
  static_assert(total_size % sizeof(T) == 0);
  static constexpr auto chunk_size = total_size / sizeof(T);
  static constexpr auto array_size = common::div_ceil(N, chunk_size);
  using chunk_ptr = std::shared_ptr<std::array<T, chunk_size>>;
  std::array<std::pair<std::size_t, chunk_ptr>, array_size> chunks;
  static inline std::size_t latest_version = 0;
  mutable std::size_t version;

  static constexpr auto index(std::size_t idx) {
    auto cidx = idx >> std::countr_zero(chunk_size);
    auto eidx = idx & (chunk_size - 1);
    return std::pair(cidx, eidx);
  }

public:
  persistent_array() : chunks{}, version(++latest_version) {
    for (auto& [ver, ptr] : chunks) {
      ver = version;
      ptr = std::make_shared<std::array<T, chunk_size>>();
    }
  }
  persistent_array(persistent_array const& ar)
      : chunks(ar.chunks), version(++latest_version) {
    ar.version = ++latest_version;
  }
  persistent_array& operator=(persistent_array const& ar) {
    chunks = ar.chunks;
    version = ++latest_version;
    ar.version = ++latest_version;
    return *this;
  }
  persistent_array(persistent_array&& ar)
      : chunks(std::move(chunks)), version(ar.version) {}
  persistent_array& operator=(persistent_array&& ar) {
    chunks = std::move(ar.chunks);
    version = ar.version;
    return *this;
  }

  template <typename U> void update(std::size_t idx, U&& value) {
    auto [cidx, eidx] = index(idx);
    auto& [ver, ptr] = chunks[cidx];
    if (std::exchange(ver, version) != version) {
      ptr = std::make_shared<std::array<T, chunk_size>>(*ptr);
    }
    (*ptr)[eidx] = std::forward<U>(value);
  }
  T const& get(std::size_t idx) const {
    auto [cidx, eidx] = index(idx);
    return (*chunks[cidx].second)[eidx];
  }
  struct iterator {
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using iterator_concept = std::input_iterator_tag;
    static constexpr auto end_cidx = N >> std::countr_zero(chunk_size);
    static constexpr auto end_eidx = N & (chunk_size - 1);
    persistent_array const* owner;
    std::size_t cidx, eidx;
    iterator(persistent_array const* p) : owner(p), cidx(), eidx() {}
    iterator& operator++() {
      ++eidx;
      if (eidx == chunk_size) {
        ++cidx;
        eidx = 0;
      }
      return *this;
    }
    iterator operator++(int) {
      auto ret = *this;
      operator++();
      return ret;
    }
    bool operator==(std::default_sentinel_t) const {
      return cidx == end_cidx && eidx == end_eidx;
    }
    const T& operator*() const {
      return (*owner->chunks[cidx].second)[eidx];
    }
  };
  std::size_t size() const {
    return N;
  }
  auto begin() const {
    return iterator(this);
  }
  auto end() const {
    return std::default_sentinel;
  }
};
} // namespace heuristic
