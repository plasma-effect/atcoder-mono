#include "competitive/heuristic/shared_array.hpp"
#include <gtest/gtest.h>

TEST(HeuristicSharedArray, DirectAccess) {
  heuristic::shared_array<int, 4> ar;
  ar[0] = 1;
  ar[1] = 2;
  ar[2] = 3;
  ar[3] = 4;
  EXPECT_EQ(ar[0], 1);
  EXPECT_EQ(ar[1], 2);
  EXPECT_EQ(ar[2], 3);
  EXPECT_EQ(ar[3], 4);
}

TEST(HeuristicSharedArray, CopyAndReuse) {
  using array_t = heuristic::shared_array<int, 8>;
  array_t ar0;
  array_t ar1 = ar0;
  array_t ar2;
  EXPECT_EQ(ar0.data(), ar1.data());
  EXPECT_NE(ar0.data(), ar2.data());
  EXPECT_NE(ar1.data(), ar2.data());

  auto before = ar2.data();
  ar2 = ar1;
  EXPECT_EQ(ar0.data(), ar2.data());
  EXPECT_EQ(ar1.data(), ar2.data());

  array_t ar3;
  EXPECT_EQ(ar3.data(), before);
}

TEST(HeuristicSharedArray, RangeBasedFor) {
  heuristic::shared_array<int, 16> ar;
  int v = 0;
  for (auto& u : ar) {
    u = ++v;
  }
  for (auto& u : ar | std::views::reverse) {
    EXPECT_EQ(u, v);
    --v;
  }
}

TEST(HeuristicSharedArray, UseAlgorithm) {
  heuristic::shared_array<std::int64_t, 4> ar = {1, 3, 5, 7};
  EXPECT_EQ(*std::lower_bound(ar.begin(), ar.end(), 4), 5);
  EXPECT_EQ(*std::find(ar.rbegin(), ar.rend(), 3), 3);
  EXPECT_EQ(*std::ranges::lower_bound(ar, 4), 5);
}

TEST(HeuristicSharedArray, AliveLimit) {
  using array_t = heuristic::shared_array<int, 2, 4>;
  ASSERT_DEATH(
      {
        array_t ar0;
        array_t ar1;
        array_t ar2;
        array_t ar3;
        array_t ar4;
      },
      ".*");
  ASSERT_EXIT(
      {
        array_t ar0;
        array_t ar1;
        array_t ar2;
        array_t ar3;
        ar2 = ar3;
        array_t ar4;
        std::exit(0);
      },
      ::testing::ExitedWithCode(0), ".*");
}
