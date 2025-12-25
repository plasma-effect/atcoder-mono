#include "competitive/data_structure/static_dsu.hpp"
#include "gtest/gtest.h"

TEST(StaticDsu, Simple) {
  competitive::static_dsu<2> uf;
  ASSERT_FALSE(uf.same(0, 1));
  int x = uf.merge(0, 1);
  ASSERT_EQ(uf.leader(0), x);
  ASSERT_EQ(uf.leader(1), x);
  ASSERT_TRUE(uf.same(0, 1));
  ASSERT_EQ(uf.size(0), 2);
}

TEST(StaticDsu, Line) {
  constexpr int n = 200000;
  competitive::static_dsu<n> uf;
  for (int i = 0; i < n - 1; ++i) {
    uf.merge(i, i + 1);
  }
  ASSERT_EQ(uf.size(0), n);
}

TEST(StaticDsu, LineReverse) {
  constexpr int n = 200000;
  competitive::static_dsu<n> uf;
  for (int i = n - 2; i >= 0; --i) {
    uf.merge(i, i + 1);
  }
  ASSERT_EQ(uf.size(0), n);
}

TEST(StaticDsu, Shuffle) {
  constexpr int n = 200000;
  competitive::static_dsu<n> uf;
  std::vector<int> vec(n);
  std::ranges::iota(vec, 0);
  std::ranges::shuffle(vec, std::mt19937());
  for (int i = 0; i < n - 1; ++i) {
    uf.merge(vec[i], vec[i + 1]);
  }
  ASSERT_EQ(uf.size(0), n);
}

TEST(StaticDsu, OoBAccess) {
  competitive::static_dsu<4> uf;
  ASSERT_DEATH({ uf.merge(-1, -1); }, ".");
  ASSERT_DEATH({ uf.merge(-1, 0); }, ".");
  ASSERT_DEATH({ uf.merge(-1, 4); }, ".");
  ASSERT_DEATH({ uf.merge(0, -1); }, ".");
  ASSERT_DEATH({ uf.merge(0, 4); }, ".");
  ASSERT_DEATH({ uf.merge(4, -1); }, ".");
  ASSERT_DEATH({ uf.merge(4, 0); }, ".");
  ASSERT_DEATH({ uf.merge(4, 4); }, ".");

  ASSERT_DEATH({ uf.same(-1, -1); }, ".");
  ASSERT_DEATH({ uf.same(-1, 0); }, ".");
  ASSERT_DEATH({ uf.same(-1, 4); }, ".");
  ASSERT_DEATH({ uf.same(0, -1); }, ".");
  ASSERT_DEATH({ uf.same(0, 4); }, ".");
  ASSERT_DEATH({ uf.same(4, -1); }, ".");
  ASSERT_DEATH({ uf.same(4, 0); }, ".");
  ASSERT_DEATH({ uf.same(4, 4); }, ".");

  ASSERT_DEATH({ uf.leader(-1); }, ".");
  ASSERT_DEATH({ uf.leader(4); }, ".");

  ASSERT_DEATH({ uf.size(-1); }, ".");
  ASSERT_DEATH({ uf.size(4); }, ".");
}
