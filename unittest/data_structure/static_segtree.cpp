#include "competitive/data_structure/static_segtree.hpp"
#include <gtest/gtest.h>

static_assert(competitive::internal::bit_ceil(1) == 1);
static_assert(competitive::internal::bit_ceil(2) == 2);
static_assert(competitive::internal::bit_ceil(3) == 4);
static_assert(competitive::internal::bit_ceil(4) == 4);

TEST(StaticSegTree, Basic) {
  competitive::static_segtree<competitive::plus<int>, 4> tree;
  tree.set(0, 0b0001);
  tree.set(1, 0b0010);
  tree.set(2, 0b0100);
  tree.set(3, 0b1000);
  EXPECT_EQ(tree.prod(0, 0), 0b0000);
  EXPECT_EQ(tree.prod(0, 1), 0b0001);
  EXPECT_EQ(tree.prod(0, 2), 0b0011);
  EXPECT_EQ(tree.prod(0, 3), 0b0111);
  EXPECT_EQ(tree.prod(0, 4), 0b1111);

  EXPECT_EQ(tree.prod(1, 1), 0b0000);
  EXPECT_EQ(tree.prod(1, 2), 0b0010);
  EXPECT_EQ(tree.prod(1, 3), 0b0110);
  EXPECT_EQ(tree.prod(1, 4), 0b1110);

  EXPECT_EQ(tree.prod(2, 2), 0b0000);
  EXPECT_EQ(tree.prod(2, 3), 0b0100);
  EXPECT_EQ(tree.prod(2, 4), 0b1100);

  EXPECT_EQ(tree.prod(3, 3), 0b0000);
  EXPECT_EQ(tree.prod(3, 4), 0b1000);

  EXPECT_EQ(tree.prod(4, 4), 0b0000);

  EXPECT_EQ(tree.all_prod(), 0b1111);
}

TEST(StaticSegTree, OutOfBounds) {
  competitive::static_segtree<competitive::plus<int>, 4> tree;
  EXPECT_DEATH({ tree.set(-1, 0); }, ".");
  EXPECT_DEATH({ tree.set(4, 0); }, ".");

  EXPECT_DEATH({ tree.get(-1); }, ".");
  EXPECT_DEATH({ tree.get(4); }, ".");

  EXPECT_DEATH({ tree.prod(-1, 0); }, ".");
  EXPECT_DEATH({ tree.prod(1, 0); }, ".");
  EXPECT_DEATH({ tree.prod(0, 5); }, ".");
}
