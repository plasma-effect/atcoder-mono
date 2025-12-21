#include "competitive/io/normal_print.hpp"
#include "competitive/utility.hpp"
#include <gtest/gtest.h>
using testing::internal::CaptureStdout;
using testing::internal::GetCapturedStdout;

TEST(NormalPrint, Integer1) {
  CaptureStdout();
  common::println(1);
  std::cout << std::flush;
  EXPECT_EQ(GetCapturedStdout(), "1\n");
}

TEST(NormalPrint, Integer2) {
  CaptureStdout();
  common::println(1, 2);
  std::cout << std::flush;
  EXPECT_EQ(GetCapturedStdout(), "1 2\n");
}

#ifdef __GNUC__
TEST(NormalPrint, ExtendedInteger) {
  CaptureStdout();
  __int128 v0 = -1;
  unsigned __int128 v1 = 2;
  common::println(v0, v1);
  EXPECT_EQ(GetCapturedStdout(), "-1 2\n");
}
#endif

TEST(NormalPrint, IntVector) {
  CaptureStdout();
  std::vector vec = {1, 2, 3};
  common::println(vec);
  std::cout << std::flush;
  EXPECT_EQ(GetCapturedStdout(), "1 2 3\n");
}

TEST(NormalPrint, Irange) {
  CaptureStdout();
  common::println(common::irange(5));
  std::cout << std::flush;
  EXPECT_EQ(GetCapturedStdout(), "0 1 2 3 4\n");
}

TEST(NormalPrint, Pair) {
  CaptureStdout();
  common::println(std::make_pair(1, 2.5));
  std::cout << std::flush;
  EXPECT_EQ(GetCapturedStdout(), "1 2.5\n");
}

TEST(NormalPrint, Tuple) {
  CaptureStdout();
  common::println(std::make_tuple(1, 2.5, "3"));
  std::cout << std::flush;
  EXPECT_EQ(GetCapturedStdout(), "1 2.5 3\n");
}

TEST(NormalPrint, TupleVector) {
  CaptureStdout();
  std::vector vec = {
      std::make_tuple(1, 2, 3),
      std::make_tuple(4, 5, 6),
  };
  common::println(vec);
  std::cout << std::flush;
  EXPECT_EQ(GetCapturedStdout(), "1 2 3 4 5 6\n");
}

TEST(NormalPrint, EmptyVector) {
  CaptureStdout();
  std::vector<int> vec = {};
  common::println(vec);
  std::cout << std::flush;
  EXPECT_EQ(GetCapturedStdout(), "\n");
}

TEST(NormalPrint, UseManip) {
  CaptureStdout();
  common::println(std::boolalpha, true);
  std::cout << std::flush;
  EXPECT_EQ(GetCapturedStdout(), "true\n");
}
