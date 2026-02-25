#include "competitive/heuristic/singleton_dual_array.hpp"
#include "gtest/gtest.h"

using dual_array = heuristic::singleton_dual_array<int, 2, 3>;

TEST(HeuristicSingletonDualArray, AssignAndRead) {
  dual_array ar;
  ar(0, 0) = 1;
  ar(0, 1) = 2;
  ar(1, 0) = 4;
  ar(1, 1) = 5;
  ASSERT_EQ(ar(0, 0), 1);
  ASSERT_EQ(ar(0, 1), 2);
  ASSERT_EQ(ar(0, 2), 0);
  ASSERT_EQ(ar(1, 0), 4);
  ASSERT_EQ(ar(1, 1), 5);
  ASSERT_EQ(ar(1, 2), 0);
}

TEST(HeuristicSingletonDualArray, OutOfBound) {
  dual_array ar;
  ASSERT_DEATH({ ar(2, 4); }, ".");
  ASSERT_DEATH({ ar(2, 0); }, ".");
  ASSERT_DEATH({ ar(2, -1); }, ".");
  ASSERT_DEATH({ ar(0, 4); }, ".");
  ASSERT_DEATH({ ar(0, -1); }, ".");
  ASSERT_DEATH({ ar(-1, 4); }, ".");
  ASSERT_DEATH({ ar(-1, 0); }, ".");
  ASSERT_DEATH({ ar(-1, -1); }, ".");
}

TEST(HeuristicSingletonDualArray, ResetByRedefine) {
  {
    dual_array ar;
    ar(0, 0) = 1;
    ar(0, 1) = 2;
    ar(1, 0) = 3;
    ar(1, 1) = 4;
    ASSERT_EQ(ar(0, 0), 1);
    ASSERT_EQ(ar(0, 1), 2);
    ASSERT_EQ(ar(1, 0), 3);
    ASSERT_EQ(ar(1, 1), 4);
  }
  {
    dual_array ar;
    ASSERT_EQ(ar(0, 0), 0);
    ASSERT_EQ(ar(0, 1), 0);
    ASSERT_EQ(ar(1, 0), 0);
    ASSERT_EQ(ar(1, 1), 0);
  }
}

#ifdef LOCAL_DEBUG
TEST(HeuristicSingletonDualArray, DualDefined) {
  dual_array ar0;
  ASSERT_DEATH({ dual_array ar1; }, ".");
}
#endif
