#include "competitive/algorithm/get_tree_diam.hpp"
#include "test_utils.hpp"
#include "gtest/gtest.h"

TEST(GetTreeDiam, Star) {
  std::vector<std::set<int>> tree(5);
  test_utils::connect(tree, 0, 1);
  test_utils::connect(tree, 1, 2);
  test_utils::connect(tree, 1, 3);
  test_utils::connect(tree, 1, 4);
  int c;
  auto d = competitive::get_tree_diam(c, tree);
  EXPECT_EQ(c, 1);
  EXPECT_EQ(d, 2);
}

TEST(GetTreeDiam, Odd) {
  std::vector<std::set<int>> tree(6);
  test_utils::connect(tree, 0, 1);
  test_utils::connect(tree, 1, 2);
  test_utils::connect(tree, 1, 3);
  test_utils::connect(tree, 3, 4);
  test_utils::connect(tree, 3, 5);
  int c;
  auto d = competitive::get_tree_diam(c, tree);
  EXPECT_TRUE(c == 1 || c == 3);
  EXPECT_EQ(d, 3);
}
