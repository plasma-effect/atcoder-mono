#include "competitive/utility/assert.hpp"
#include <gtest/gtest.h>

TEST(Assertion, Macro) {
#ifdef LOCAL_DEBUG
  common::debug::internal::location _from = std::source_location::current();
#endif
  CL_ASSERT(true);
  ASSERT_DEATH(CL_ASSERT(false), ".");
}

TEST(ArgumentWrapper, DebugPrint) {
  common::argument<int> arg = 1;
  testing::internal::CaptureStderr();
  common::debug::println(arg);
  EXPECT_EQ(testing::internal::GetCapturedStderr(), "1\n");
}
