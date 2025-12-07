#pragma once
#include "atcoder/all"
#include "competitive/math/monoid.hpp"

namespace competitive {
namespace internal {
template <auto monoid> using value_t = typename decltype(monoid)::value_t;
} // namespace internal
template <auto monoid>
using segtree = atcoder::segtree<internal::value_t<monoid>, monoid, monoid>;
template <typename T> using add_segtree = segtree<plus<T>>;
template <typename T> using mul_segtree = segtree<multiplies<T>>;
template <typename T> using max_segtree = segtree<max<T>>;

template <auto monoid, auto mapping>
using lazy_segtree = atcoder::lazy_segtree<internal::value_t<monoid>, monoid,
                                           monoid, internal::value_t<mapping>,
                                           mapping, mapping, mapping>;
template <typename T> struct seq_update_lsegtree {
  static auto add(common::pair<T> const& lhs, common::pair<T> const& rhs) {
    return common::pair(lhs.first + rhs.first, lhs.second + rhs.second);
  }
  static common::pair<T> e() {
    return {T(), T(1)};
  }
  static common::pair<T> mapping(std::optional<T> const& f,
                                 common::pair<T> const& p) {
    if (f) {
      return {*f * p.second, p.second};
    } else {
      return p;
    }
  }
  static std::optional<T> composition(std::optional<T> const& f,
                                      std::optional<T> const& g) {
    return f ? f : g;
  }
  static std::optional<T> id() {
    return std::nullopt;
  }
  atcoder::lazy_segtree<common::pair<T>, add, e, std::optional<T>, mapping,
                        composition, id>
      tree;
  explicit seq_update_lsegtree(int N) : tree(N) {}
  void set(int p, T x) {
    tree.set(p, {x, 1});
  }
  T get(int p) {
    return tree.get(p).first;
  }
  T prod(int l, int r) {
    return tree.prod(l, r).first;
  }
  T all_prod() {
    return tree.all_prod().first;
  }
  void apply(int p, T f) {
    tree.apply(p, f);
  }
  void apply(int l, int r, T f) {
    tree.apply(l, r, f);
  }
};
} // namespace competitive
