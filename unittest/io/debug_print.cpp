#include "competitive/io/debug_print.hpp"
#include "competitive/utility.hpp"
#include <gtest/gtest.h>
using testing::internal::CaptureStderr;
using testing::internal::GetCapturedStderr;

TEST(DebugPrint, Integer1) {
  CaptureStderr();
  common::debug::println(1);
  EXPECT_EQ(GetCapturedStderr(), "1\n");
}

TEST(DebugPrint, Integer2) {
  CaptureStderr();
  common::debug::println(1, 2);
  EXPECT_EQ(GetCapturedStderr(), "1 2\n");
}

#ifdef __GNUC__
TEST(DebugPrint, ExtendedInteger) {
  CaptureStderr();
  __int128 v0 = -1;
  unsigned __int128 v1 = 2;
  common::debug::println(v0, v1);
  EXPECT_EQ(GetCapturedStderr(), "-1 2\n");
}
#endif

TEST(DebugPrint, IntVector) {
  CaptureStderr();
  std::vector vec = {1, 2, 3};
  common::debug::println(vec);
  EXPECT_EQ(GetCapturedStderr(), "{1, 2, 3}\n");
}

TEST(DebugPrint, Irange) {
  CaptureStderr();
  common::debug::println(common::irange(5));
  EXPECT_EQ(GetCapturedStderr(), "{0, 1, 2, 3, 4}\n");
}

TEST(DebugPrint, Pair) {
  CaptureStderr();
  common::debug::println(std::make_pair(1, 2.5));
  EXPECT_EQ(GetCapturedStderr(), "(1, 2.5)\n");
}

TEST(DebugPrint, Tuple) {
  CaptureStderr();
  common::debug::println(std::make_tuple(1, 2.5, "3"));
  EXPECT_EQ(GetCapturedStderr(), "(1, 2.5, 3)\n");
}

TEST(DebugPrint, TupleVector) {
  CaptureStderr();
  std::vector vec = {
      std::make_tuple(1, 2, 3),
      std::make_tuple(4, 5, 6),
  };
  common::debug::println(vec);
  EXPECT_EQ(GetCapturedStderr(), "{(1, 2, 3), (4, 5, 6)}\n");
}

TEST(DebugPrint, UseManip) {
  CaptureStderr();
  common::debug::println(std::boolalpha, true);
  EXPECT_EQ(GetCapturedStderr(), "true\n");
}

TEST(DebugPrint, DebugPrintMacro) {
  CaptureStderr();
  DEBUG_PRINT(1, 2);
#ifdef LOCAL_DEBUG
  EXPECT_EQ(GetCapturedStderr(), "72: 1 2\n");
#else
  EXPECT_EQ(GetCapturedStderr(), "");
#endif
}

TEST(DebugPrint, DebugPrintMacroNone) {
  CaptureStderr();
  DEBUG_PRINT();
#ifdef LOCAL_DEBUG
  EXPECT_EQ(GetCapturedStderr(), "82:\n");
#else
  EXPECT_EQ(GetCapturedStderr(), "");
#endif
}
