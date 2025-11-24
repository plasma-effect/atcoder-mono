#include "competitive/utility/assert.hpp"
#include <gtest/gtest.h>

TEST(Assertion, Macro) {
#ifdef LOCAL_DEBUG
  common::debug::internal::location _from = std::source_location::current();
#endif
  CL_ASSERT(true);
  ASSERT_DEATH(CL_ASSERT(false), ".");
}
