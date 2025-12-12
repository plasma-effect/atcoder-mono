#include "competitive/heuristic/singleton_dual_array.hpp"
#include "gtest/gtest.h"

using dual_array = heuristic::singleton_dual_array<int, 2, 3>;

TEST(HeuristicSingletonDualArray, AssignAndRead) {
  dual_array ar;
  ar.set(0, 0, 1);
  ar.set(0, 1, 2);
  ar.set(1, 0, 4);
  ar.set(1, 1, 5);
  ASSERT_EQ(ar.get(0, 0), 1);
  ASSERT_EQ(ar.get(0, 1), 2);
  ASSERT_EQ(ar.get(0, 2), 0);
  ASSERT_EQ(ar.get(1, 0), 4);
  ASSERT_EQ(ar.get(1, 1), 5);
  ASSERT_EQ(ar.get(1, 2), 0);
}

TEST(HeuristicSingletonDualArray, OutOfBound) {
  dual_array ar;
  ASSERT_DEATH({ ar.set(2, 4, 0); }, ".");
  ASSERT_DEATH({ ar.set(2, 0, 0); }, ".");
  ASSERT_DEATH({ ar.set(2, -1, 0); }, ".");
  ASSERT_DEATH({ ar.set(0, 4, 0); }, ".");
  ASSERT_DEATH({ ar.set(0, -1, 0); }, ".");
  ASSERT_DEATH({ ar.set(-1, 4, 0); }, ".");
  ASSERT_DEATH({ ar.set(-1, 0, 0); }, ".");
  ASSERT_DEATH({ ar.set(-1, -1, 0); }, ".");

  ASSERT_DEATH({ ar.get(2, 4); }, ".");
  ASSERT_DEATH({ ar.get(2, 0); }, ".");
  ASSERT_DEATH({ ar.get(2, -1); }, ".");
  ASSERT_DEATH({ ar.get(0, 4); }, ".");
  ASSERT_DEATH({ ar.get(0, -1); }, ".");
  ASSERT_DEATH({ ar.get(-1, 4); }, ".");
  ASSERT_DEATH({ ar.get(-1, 0); }, ".");
  ASSERT_DEATH({ ar.get(-1, -1); }, ".");
}

TEST(HeuristicSingletonDualArray, ResetByRedefine) {
  {
    dual_array ar;
    ar.set(0, 0, 1);
    ar.set(0, 1, 2);
    ar.set(1, 0, 3);
    ar.set(1, 1, 4);
    ASSERT_EQ(ar.get(0, 0), 1);
    ASSERT_EQ(ar.get(0, 1), 2);
    ASSERT_EQ(ar.get(1, 0), 3);
    ASSERT_EQ(ar.get(1, 1), 4);
  }
  {
    dual_array ar;
    ASSERT_EQ(ar.get(0, 0), 0);
    ASSERT_EQ(ar.get(0, 1), 0);
    ASSERT_EQ(ar.get(1, 0), 0);
    ASSERT_EQ(ar.get(1, 1), 0);
  }
}

#ifdef LOCAL_DEBUG
TEST(HeuristicSingletonDualArray, DualDefined) {
  dual_array ar0;
  ASSERT_DEATH(
      { dual_array ar1; },
      "\"heuristic::singleton_dual_array<int, 2, 3, -1>\" is alive.*");
}
#endif
