#include "competitive/algorithm/rolling_hash.hpp"
#include "gtest/gtest.h"

TEST(RollingHash, Utilities) {
  using hash_t = competitive::rolling_hash<'a', 'z'>;
  hash_t h0("abc");
  auto raw0 = h0.raw();
  EXPECT_EQ(h0.size(), 3);

  static_assert(!std::is_copy_constructible_v<hash_t>);
  static_assert(!std::is_copy_assignable_v<hash_t>);
  auto h1 = std::move(h0);
  EXPECT_EQ(h1.raw(), raw0);
  h0 = std::move(h1);
  EXPECT_EQ(h0.raw(), raw0);
}

TEST(RollingHash, Parent) {
  using hash_t = competitive::rolling_hash<'a', 'z'>;
  std::string s0 = "abc";
  std::string s1 = "abcd";
  std::string s2 = "abd";
  hash_t h0(s0);
  hash_t h1(s0);
  hash_t h2(s1);
  hash_t h3(s2);
  EXPECT_EQ(h0, h1);
  EXPECT_NE(h0, h2);
  EXPECT_NE(h0, h3);
}

TEST(RollingHash, UsingViews) {
  using hash_t = competitive::rolling_hash<'a', 'z'>;
  std::string s0 = "abc";
  std::string s1 = "cba";
  hash_t h0(s0);
  hash_t h1(s1 | std::views::reverse);
  EXPECT_EQ(h0, h1);
}

TEST(RollingHash, SubHash) {
  using hash_t = competitive::rolling_hash<'a', 'z'>;
  std::string s0 = "abcabc";
  hash_t h0(s0);
  auto sh0 = h0.subhash(0, 3);
  auto sh1 = h0.subhash(1, 3);
  auto sh2 = h0.subhash(3, 3);
  auto sh3 = h0.subhash(0);
  auto sh4 = h0.subhash(3);
  EXPECT_NE(sh0, sh1);
  EXPECT_EQ(sh0, sh2);
  EXPECT_NE(sh0, sh3);
  EXPECT_EQ(sh0, sh4);

  // rolling_hash can compare to subhash_t
  EXPECT_EQ(h0, sh3);
  EXPECT_EQ(sh3, h0);
}

TEST(RollingHash, Set) {
  using hash_t = competitive::rolling_hash<'a', 'z'>;
  std::set<hash_t> set;
  set.emplace("abc");
  set.emplace("def");
  set.emplace("ghi");
  EXPECT_EQ(set.size(), 3);
  EXPECT_TRUE(set.contains("abc"));
  EXPECT_FALSE(set.contains("bcd"));
}

TEST(RollingHash, UnorderedSet) {
  using hash_t = competitive::rolling_hash<'a', 'z'>;
  std::unordered_set<hash_t> set;
  set.emplace("abc");
  set.emplace("def");
  set.emplace("ghi");
  EXPECT_EQ(set.size(), 3);
  EXPECT_TRUE(set.contains("abc"));
  EXPECT_FALSE(set.contains("bcd"));
}
