#pragma once
#include "atcoder/all"
#include <bits/stdc++.h>

namespace common::internal {
template <typename T>
concept stdstream_able = requires(T a) { std::declval<std::ostream&>() << a; };

class print_base_t {
  std::ios_base::fmtflags base_flags;
  std::ostream& ost;
  using cstr = const char*;
  cstr rng_prefix;
  cstr rng_suffix;
  cstr rng_delim;
  cstr tpl_prefix;
  cstr tpl_suffix;
  cstr tpl_delim;

public:
  print_base_t(std::ostream& os, cstr (&rng)[3], cstr (&tpl)[3])
      : base_flags(os.flags()), ost(os), rng_prefix{rng[0]}, rng_suffix{rng[1]},
        rng_delim{rng[2]}, tpl_prefix{tpl[0]}, tpl_suffix{tpl[1]},
        tpl_delim{tpl[2]} {}
  ~print_base_t() {
    ost.flags(base_flags);
  }

  void operator<<(std::string const& str) {
    ost << str;
  }
  void operator<<(std::string_view const& view) {
    ost << view;
  }
  void operator<<(const char* str) {
    ost << str;
  }
#ifdef __GNUC__
  void operator<<(__int128 v) {
    ost << static_cast<std::int64_t>(v);
  }
  void operator<<(unsigned __int128 v) {
    ost << static_cast<std::int64_t>(v);
  }
#endif
  template <int mod> void operator<<(atcoder::static_modint<mod> const& val) {
    ost << val.val();
  }
  template <stdstream_able T> void operator<<(T const& v) {
    ost << v;
  }
  template <std::input_iterator It> void print_ite(It first, It last) {
    const char* d = rng_prefix;
    for (; first != last; ++first) {
      ost << std::exchange(d, rng_delim);
      *this << *first;
    }
    ost << rng_suffix;
  }
  template <std::ranges::input_range T> void operator<<(T const& rng) {
    print_ite(rng.begin(), rng.end());
  }
  template <typename T, std::size_t N> void operator<<(T const (&ar)[N]) {
    print_ite(std::ranges::begin(ar), std::ranges::end(ar));
  }
  template <std::size_t S, std::size_t I, typename T>
  void tuple_print(T const& t) {
    ost << (I == 0 ? tpl_prefix : tpl_delim);
    *this << std::get<I>(t);
    if constexpr (I + 1 != S) {
      tuple_print<S, I + 1>(t);
    } else {
      ost << tpl_suffix;
    }
  }
  template <typename T0, typename T1>
  void operator<<(std::pair<T0, T1> const& p) {
    tuple_print<2, 0>(p);
  }
  template <typename... Ts> void operator<<(std::tuple<Ts...> const& t) {
    tuple_print<sizeof...(Ts), 0>(t);
  }
  template <typename T> void operator<<(std::optional<T> const& opt) {
    if (opt) {
      *this << *opt;
    } else {
      ost << "<nullopt>";
    }
  }
  std::array<const char*, 3> get_range_decolater() {
    return {
        rng_prefix,
        rng_suffix,
        rng_delim,
    };
  }
  std::array<const char*, 3> get_tuple_decolater() {
    return {
        tpl_prefix,
        tpl_suffix,
        tpl_delim,
    };
  }
};

template <typename T>
constexpr bool is_std_manip_v =
    std::is_same_v<T, decltype(std::setbase(std::declval<int>()))> ||
    std::is_same_v<T, decltype(std::setfill(std::declval<char>()))> ||
    std::is_same_v<T, decltype(std::setprecision(std::declval<int>()))> ||
    std::is_same_v<T, decltype(std::setw(std::declval<int>()))> ||
    std::is_convertible_v<T, std::ios_base& (*)(std::ios_base&)>;

template <bool> void print(print_base_t&) {}
template <bool put_blank, typename T, typename... Ts>
void print(print_base_t& pb, T const& arg, Ts const&... args) {
  if constexpr (put_blank) {
    pb << " ";
  }
  pb << arg;
  print<!is_std_manip_v<std::remove_cv_t<T>>>(pb, args...);
}
} // namespace common::internal
