#include "competitive/heuristic/persistent_array.hpp"
#include <gtest/gtest.h>

TEST(HeuristicPersistentArray, General) {
  heuristic::persistent_array<int, 4> ar;
  ar.update(0, 1);
  ar.update(1, 3);
  ar.update(2, 5);
  ar.update(3, 7);
  ASSERT_EQ(ar.size(), 4);
  EXPECT_EQ(ar.get(0), 1);
  EXPECT_EQ(ar.get(1), 3);
  EXPECT_EQ(ar.get(2), 5);
  EXPECT_EQ(ar.get(3), 7);
}

TEST(HeuristicPersistentArray, BranchingHistory) {
  heuristic::persistent_array<int, 6> ar0;
  ar0.update(0, 1);
  ar0.update(1, 3);
  auto ar1 = ar0;
  ar0.update(2, 4);
  ar1.update(2, 5);
  ar0.update(3, 6);
  ar1.update(3, 7);
  auto ar2 = ar1;
  ar1.update(4, 8);
  ar2.update(4, 9);
  ar1.update(5, 10);
  ar2.update(5, 11);

  EXPECT_EQ(ar0.get(0), 1);
  EXPECT_EQ(ar0.get(1), 3);
  EXPECT_EQ(ar0.get(2), 4);
  EXPECT_EQ(ar0.get(3), 6);
  EXPECT_EQ(ar0.get(4), 0);
  EXPECT_EQ(ar0.get(5), 0);

  EXPECT_EQ(ar1.get(0), 1);
  EXPECT_EQ(ar1.get(1), 3);
  EXPECT_EQ(ar1.get(2), 5);
  EXPECT_EQ(ar1.get(3), 7);
  EXPECT_EQ(ar1.get(4), 8);
  EXPECT_EQ(ar1.get(5), 10);

  EXPECT_EQ(ar2.get(0), 1);
  EXPECT_EQ(ar2.get(1), 3);
  EXPECT_EQ(ar2.get(2), 5);
  EXPECT_EQ(ar2.get(3), 7);
  EXPECT_EQ(ar2.get(4), 9);
  EXPECT_EQ(ar2.get(5), 11);
}

TEST(HeuristicPersistentArray, MemoryExpired) {
  heuristic::persistent_array<std::shared_ptr<int>, 4> ar;
  ar.update(0, std::make_shared<int>(0));
  ar.update(1, std::make_shared<int>(1));
  ar.update(2, std::make_shared<int>(2));
  ar.update(3, std::make_shared<int>(3));
  std::weak_ptr wp0 = ar.get(0);
  std::weak_ptr wp1 = ar.get(1);
  std::weak_ptr wp2 = ar.get(2);
  std::weak_ptr wp3 = ar.get(3);
  EXPECT_FALSE(wp0.expired());
  EXPECT_FALSE(wp1.expired());
  EXPECT_FALSE(wp2.expired());
  EXPECT_FALSE(wp3.expired());

  ar.update(0, std::make_shared<int>(4));
  EXPECT_TRUE(wp0.expired());
  EXPECT_FALSE(wp1.expired());
  EXPECT_FALSE(wp2.expired());
  EXPECT_FALSE(wp3.expired());

  ar.update(1, std::make_shared<int>(5));
  EXPECT_TRUE(wp0.expired());
  EXPECT_TRUE(wp1.expired());
  EXPECT_FALSE(wp2.expired());
  EXPECT_FALSE(wp3.expired());

  ar.update(2, std::make_shared<int>(6));
  EXPECT_TRUE(wp0.expired());
  EXPECT_TRUE(wp1.expired());
  EXPECT_TRUE(wp2.expired());
  EXPECT_FALSE(wp3.expired());

  ar.update(3, std::make_shared<int>(7));
  EXPECT_TRUE(wp0.expired());
  EXPECT_TRUE(wp1.expired());
  EXPECT_TRUE(wp2.expired());
  EXPECT_TRUE(wp3.expired());
}

TEST(HeuristicPersistentArray, Iteration) {
  heuristic::persistent_array<int, 4> ar;
  ar.update(0, 1);
  ar.update(1, 3);
  ar.update(2, 5);
  ar.update(3, 7);
  std::vector vec = {1, 3, 5, 7};
  EXPECT_TRUE(std::ranges::equal(ar, vec));
}

namespace {
template <typename T>
using large_array_t = heuristic::persistent_array<T, 1024>;
static constexpr auto chunk_size = 256;
} // namespace
TEST(HeuristicPersistentArrayLarge, General) {
  large_array_t<int> ar;
  ar.update(0 * chunk_size, 1);
  ar.update(1 * chunk_size, 3);
  ar.update(2 * chunk_size, 5);
  ar.update(3 * chunk_size, 7);
  ASSERT_EQ(ar.size(), 1024);
  EXPECT_EQ(ar.get(0 * chunk_size), 1);
  EXPECT_EQ(ar.get(1 * chunk_size), 3);
  EXPECT_EQ(ar.get(2 * chunk_size), 5);
  EXPECT_EQ(ar.get(3 * chunk_size), 7);
}

TEST(HeuristicPersistentArrayLarge, BranchingHistory) {
  large_array_t<int> ar0;
  ar0.update(0 * chunk_size, 1);
  auto ar1 = ar0;
  ar0.update(1 * chunk_size, 2);
  ar1.update(1 * chunk_size, 3);
  auto ar2 = ar1;
  ar1.update(2 * chunk_size, 4);
  ar2.update(2 * chunk_size, 5);

  EXPECT_EQ(ar0.get(0 * chunk_size), 1);
  EXPECT_EQ(ar0.get(1 * chunk_size), 2);
  EXPECT_EQ(ar0.get(2 * chunk_size), 0);

  EXPECT_EQ(ar1.get(0 * chunk_size), 1);
  EXPECT_EQ(ar1.get(1 * chunk_size), 3);
  EXPECT_EQ(ar1.get(2 * chunk_size), 4);

  EXPECT_EQ(ar2.get(0 * chunk_size), 1);
  EXPECT_EQ(ar2.get(1 * chunk_size), 3);
  EXPECT_EQ(ar2.get(2 * chunk_size), 5);
}

TEST(HeuristicPersistentArrayLarge, MemoryExpired) {
  large_array_t<std::shared_ptr<int>> ar;
  ar.update(0 * chunk_size, std::make_shared<int>(0));
  ar.update(1 * chunk_size, std::make_shared<int>(1));
  ar.update(2 * chunk_size, std::make_shared<int>(2));
  ar.update(3 * chunk_size, std::make_shared<int>(3));
  std::weak_ptr wp0 = ar.get(0 * chunk_size);
  std::weak_ptr wp1 = ar.get(1 * chunk_size);
  std::weak_ptr wp2 = ar.get(2 * chunk_size);
  std::weak_ptr wp3 = ar.get(3 * chunk_size);
  EXPECT_FALSE(wp0.expired());
  EXPECT_FALSE(wp1.expired());
  EXPECT_FALSE(wp2.expired());
  EXPECT_FALSE(wp3.expired());

  ar.update(0 * chunk_size, std::make_shared<int>(4));
  EXPECT_TRUE(wp0.expired());
  EXPECT_FALSE(wp1.expired());
  EXPECT_FALSE(wp2.expired());
  EXPECT_FALSE(wp3.expired());

  ar.update(1 * chunk_size, std::make_shared<int>(5));
  EXPECT_TRUE(wp0.expired());
  EXPECT_TRUE(wp1.expired());
  EXPECT_FALSE(wp2.expired());
  EXPECT_FALSE(wp3.expired());

  ar.update(2 * chunk_size, std::make_shared<int>(6));
  EXPECT_TRUE(wp0.expired());
  EXPECT_TRUE(wp1.expired());
  EXPECT_TRUE(wp2.expired());
  EXPECT_FALSE(wp3.expired());

  ar.update(3 * chunk_size, std::make_shared<int>(7));
  EXPECT_TRUE(wp0.expired());
  EXPECT_TRUE(wp1.expired());
  EXPECT_TRUE(wp2.expired());
  EXPECT_TRUE(wp3.expired());
}
