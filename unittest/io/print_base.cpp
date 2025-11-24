#include "competitive/io/print_base.hpp"
#include "competitive/utility.hpp"
#include <gtest/gtest.h>

namespace {
const char* default_dec[] = {"", "", " "};
}
TEST(PrintBase, String) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  std::string str = "str";
  base << str;
  EXPECT_EQ(ss.str(), "str");
}

TEST(PrintBase, StringView) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  std::string_view str = "str";
  base << str;
  EXPECT_EQ(ss.str(), "str");
}

TEST(PrintBase, CharPtr) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  const char* str = "str";
  base << str;
  EXPECT_EQ(ss.str(), "str");
}

TEST(PrintBase, CharLiteral) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  base << "str";
  EXPECT_EQ(ss.str(), "str");
}

TEST(PrintBase, Integer) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  base << 1;
  base << 2u;
  EXPECT_EQ(ss.str(), "12");
}

#ifdef __GNUC__
TEST(PrintBase, ExtendedInteger) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  base << static_cast<__int128>(-1);
  base << static_cast<unsigned __int128>(2);
  EXPECT_EQ(ss.str(), "-12");
}
#endif

TEST(PrintBase, Floating) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  base << 1.5;
  EXPECT_EQ(ss.str(), "1.5");
}

TEST(PrintBase, ExecuteManip) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  base << std::setprecision(3);
  base << 0.1234;
  EXPECT_EQ(ss.str(), "0.123");
}

TEST(PrintBase, AtCoderStaticModint) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  atcoder::static_modint<7> mod(2);
  base << mod;
  EXPECT_EQ(ss.str(), "2");
}

TEST(PrintBase, CommonRange) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  base << common::irange(5);
  EXPECT_EQ(ss.str(), "0 1 2 3 4");
}

TEST(PrintBase, StdVector) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  std::vector vec = {0, 2, 4};
  base << vec;
  EXPECT_EQ(ss.str(), "0 2 4");
}

TEST(PrintBase, StaticArray) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  int ar[] = {1, 3, 5};
  base << ar;
  EXPECT_EQ(ss.str(), "1 3 5");
}

TEST(PrintBase, Pair) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  std::pair p(1, "2");
  base << p;
  EXPECT_EQ(ss.str(), "1 2");
}

TEST(PrintBase, Tuple) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  std::tuple t(1, "2", 3u);
  base << t;
  EXPECT_EQ(ss.str(), "1 2 3");
}

TEST(PrintBase, Optional) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  std::optional<int> a;
  base << a;
  a = 1;
  base << a;
  EXPECT_EQ(ss.str(), "<nullopt>1");
}

TEST(PrintBase, OptionalVector) {
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, default_dec);
  std::vector<std::optional<int>> vec(3);
  vec[0] = 1;
  vec[2] = 3;
  base << vec;
  EXPECT_EQ(ss.str(), "1 <nullopt> 3");
}

TEST(PrintBase, SetRangeDec) {
  const char* decolater[] = {"{", "}", ","};
  std::stringstream ss;
  common::internal::print_base_t base(ss, decolater, default_dec);
  std::vector vec = {0, 1, 2};
  base << vec;
  EXPECT_EQ(ss.str(), "{0,1,2}");
}

TEST(PrintBase, SetTupleDec) {
  const char* decolater[] = {"(", ")", ","};
  std::stringstream ss;
  common::internal::print_base_t base(ss, default_dec, decolater);
  std::tuple tpl(0, 1, 2);
  base << tpl;
  EXPECT_EQ(ss.str(), "(0,1,2)");
}

TEST(PrintBase, SetCombDec) {
  const char* rng_decolater[] = {"{", "}", ", "};
  const char* tpl_decolater[] = {"(", ")", ", "};
  std::stringstream ss;
  common::internal::print_base_t base(ss, rng_decolater, tpl_decolater);
  std::vector vec = {std::pair(1, 2), std::pair(3, 4)};
  base << vec;
  EXPECT_EQ(ss.str(), "{(1, 2), (3, 4)}");
}
