#pragma once
#include <future>

namespace test_utils {
template <typename Func> auto async(Func func) {
  return std::async(std::launch::async, func).get();
}
template <typename T> struct uncopyable {
  T value;
  uncopyable() = default;
  uncopyable(T v) : value(std::move(v)) {}
  uncopyable(uncopyable const&) = delete;
  uncopyable(uncopyable&&) = default;
  uncopyable& operator=(uncopyable const&) = delete;
  uncopyable& operator=(uncopyable&&) = default;
};
template <typename T>
bool operator==(uncopyable<T> const& lhs, uncopyable<T> const& rhs) {
  return lhs.value == rhs.value;
}
template <typename T>
bool operator!=(uncopyable<T> const& lhs, uncopyable<T> const& rhs) {
  return lhs.value != rhs.value;
}
} // namespace test_utils
