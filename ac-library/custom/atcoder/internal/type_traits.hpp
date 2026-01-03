#ifndef ATCODER_INTERNAL_TYPE_TRAITS_HPP
#define ATCODER_INTERNAL_TYPE_TRAITS_HPP 1

#include <cassert>
#include <numeric>
#include <type_traits>

namespace atcoder {

namespace internal {

template <typename T>
concept signed_int128 =
    std::is_same_v<T, __int128_t> || std::is_same_v<T, __int128>;

template <typename T>
concept unsigned_int128 =
    std::is_same_v<T, __uint128_t> || std::is_same_v<T, unsigned __int128>;

template <class T>
using make_unsigned_int128 =
    typename std::conditional<std::is_same<T, __int128_t>::value,
                              __uint128_t,
                              unsigned __int128>;

template <typename T>
concept integral = std::integral<T> || signed_int128<T> || unsigned_int128<T>;

template <typename T>
concept signed_integral = std::signed_integral<T> || signed_int128<T>;

template <typename T>
concept unsigned_integral = std::unsigned_integral<T> || unsigned_int128<T>;

template <class T>
using to_unsigned = typename std::conditional<
    signed_int128<T>,
    make_unsigned_int128<T>,
    typename std::conditional<std::is_signed<T>::value,
                              std::make_unsigned<T>,
                              std::common_type<T>>::type>::type;

template <class T> using to_unsigned_t = typename to_unsigned<T>::type;

}  // namespace internal

}  // namespace atcoder

#endif  // ATCODER_INTERNAL_TYPE_TRAITS_HPP
