#include "competitive/data_structure/segtree.hpp"
#include <gtest/gtest.h>

TEST(SegTree, AddSegTreeProd) {
  std::vector base = {0b0001, 0b0010, 0b0100, 0b1000};
  competitive::add_segtree<int> tree(base);
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

TEST(SegTree, AddSegTreeMaxRight) {
  std::vector base = {0b0001, 0b0010, 0b0100, 0b1000};
  competitive::add_segtree<int> tree(base);
  EXPECT_EQ(tree.max_right(0, [&](int v) { return v <= base[0] + base[1]; }),
            2);
  EXPECT_EQ(tree.max_right(2, [&](int v) { return v <= base[2]; }), 3);
  EXPECT_EQ(tree.max_right(0, [](int) { return true; }), 4);
  EXPECT_EQ(tree.max_right(0, [](int v) { return v <= 0; }), 0);
}

TEST(SegTree, AddSegTreeMinLeft) {
  std::vector base = {0b0001, 0b0010, 0b0100, 0b1000};
  competitive::add_segtree<int> tree(base);
  EXPECT_EQ(tree.min_left(4, [&](int v) { return v <= base[3] + base[2]; }), 2);
  EXPECT_EQ(tree.min_left(2, [&](int v) { return v <= base[1]; }), 1);
  EXPECT_EQ(tree.min_left(4, [](int) { return true; }), 0);
  EXPECT_EQ(tree.min_left(4, [](int v) { return v <= 0; }), 4);
}

TEST(LazySegTree, MinAddLazySegTreeProd) {
  std::vector base = {0, 1, 2, 3};
  competitive::lazy_segtree<competitive::min<int>, competitive::plus<int>> tree(
      base);
  EXPECT_EQ(tree.prod(0, 4), 0);
  EXPECT_EQ(tree.prod(1, 4), 1);
  EXPECT_EQ(tree.prod(2, 4), 2);
  EXPECT_EQ(tree.prod(3, 4), 3);
  EXPECT_EQ(tree.prod(4, 4), INT_MAX);

  EXPECT_EQ(tree.prod(0, 3), 0);
  EXPECT_EQ(tree.prod(1, 3), 1);
  EXPECT_EQ(tree.prod(2, 3), 2);
  EXPECT_EQ(tree.prod(3, 3), INT_MAX);

  EXPECT_EQ(tree.prod(0, 2), 0);
  EXPECT_EQ(tree.prod(1, 2), 1);
  EXPECT_EQ(tree.prod(2, 2), INT_MAX);

  EXPECT_EQ(tree.prod(0, 1), 0);
  EXPECT_EQ(tree.prod(1, 1), INT_MAX);

  EXPECT_EQ(tree.prod(0, 0), INT_MAX);

  EXPECT_EQ(tree.all_prod(), 0);
}

TEST(LazySegTree, MinAddLazySegTreeApply) {
  std::vector base = {0, 0, 0, 0};
  competitive::lazy_segtree<competitive::min<int>, competitive::plus<int>> tree(
      base);
  tree.apply(0, 2, 0b0001);
  tree.apply(1, 3, 0b0010);
  tree.apply(2, 4, 0b0100);
  EXPECT_EQ(tree.get(0), 0b0001);
  EXPECT_EQ(tree.get(1), 0b0011);
  EXPECT_EQ(tree.get(2), 0b0110);
  EXPECT_EQ(tree.get(3), 0b0100);

  EXPECT_EQ(tree.prod(0, 4), tree.get(0));
  EXPECT_EQ(tree.prod(1, 4), tree.get(1));
  EXPECT_EQ(tree.prod(2, 4), tree.get(3));
  EXPECT_EQ(tree.prod(3, 4), tree.get(3));
  EXPECT_EQ(tree.prod(4, 4), INT_MAX);

  EXPECT_EQ(tree.prod(0, 3), tree.get(0));
  EXPECT_EQ(tree.prod(1, 3), tree.get(1));
  EXPECT_EQ(tree.prod(2, 3), tree.get(2));
  EXPECT_EQ(tree.prod(3, 3), INT_MAX);

  EXPECT_EQ(tree.prod(0, 2), tree.get(0));
  EXPECT_EQ(tree.prod(1, 2), tree.get(1));
  EXPECT_EQ(tree.prod(2, 2), INT_MAX);

  EXPECT_EQ(tree.prod(0, 1), tree.get(0));
  EXPECT_EQ(tree.prod(1, 1), INT_MAX);

  EXPECT_EQ(tree.prod(0, 0), INT_MAX);

  EXPECT_EQ(tree.all_prod(), tree.get(0));
}

TEST(LazySegTree, MinAddLazySegTreeMaxRight) {
  std::vector base = {3, 2, 1, 0};
  competitive::lazy_segtree<competitive::min<int>, competitive::plus<int>> tree(
      base);
  EXPECT_EQ(tree.max_right(0, [](int v) { return v >= 2; }), 2);
  EXPECT_EQ(tree.max_right(2, [](int v) { return v >= 0; }), 4);
  EXPECT_EQ(tree.max_right(0, [](int) { return true; }), 4);
  EXPECT_EQ(tree.max_right(0, [](int v) { return v >= 4; }), 0);
}

TEST(LazySegTree, MinAddLazySegTreeMinLeft) {
  std::vector base = {0, 1, 2, 3};
  competitive::lazy_segtree<competitive::min<int>, competitive::plus<int>> tree(
      base);
  EXPECT_EQ(tree.min_left(4, [](int v) { return v >= 2; }), 2);
  EXPECT_EQ(tree.min_left(2, [](int v) { return v >= 0; }), 0);
  EXPECT_EQ(tree.min_left(4, [](int) { return true; }), 0);
  EXPECT_EQ(tree.min_left(4, [](int v) { return v >= 4; }), 4);
}

TEST(SeqUpdateLazySegTree, Prod) {
  competitive::seq_update_lsegtree<int> tree(4);
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

TEST(SeqUpdateLazySegTree, Apply) {
  competitive::seq_update_lsegtree<int> tree(4);
  tree.apply(0, 2, 1);
  tree.apply(1, 3, 2);
  tree.apply(2, 4, 3);
  EXPECT_EQ(tree.get(0), 1);
  EXPECT_EQ(tree.get(1), 2);
  EXPECT_EQ(tree.get(2), 3);
  EXPECT_EQ(tree.get(3), 3);

  EXPECT_EQ(tree.prod(0, 0), 0);
  EXPECT_EQ(tree.prod(0, 1), 1);
  EXPECT_EQ(tree.prod(0, 2), 3);
  EXPECT_EQ(tree.prod(0, 3), 6);
  EXPECT_EQ(tree.prod(0, 4), 9);
  EXPECT_EQ(tree.prod(1, 1), 0);
  EXPECT_EQ(tree.prod(1, 2), 2);
  EXPECT_EQ(tree.prod(1, 3), 5);
  EXPECT_EQ(tree.prod(1, 4), 8);
  EXPECT_EQ(tree.prod(2, 2), 0);
  EXPECT_EQ(tree.prod(2, 3), 3);
  EXPECT_EQ(tree.prod(2, 4), 6);
  EXPECT_EQ(tree.prod(3, 3), 0);
  EXPECT_EQ(tree.prod(3, 4), 3);
  EXPECT_EQ(tree.prod(4, 4), 0);
}
