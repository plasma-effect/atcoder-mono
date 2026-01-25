#include "competitive/math/argument_sort.hpp"
#include <gtest/gtest.h>

TEST(Math, ArgumentSort1) {
  common::point_t points[] = {
      {2, 2},
      {2, 1},
      {2, 0},
      {1, 2},
      {1, 1},
      {1, 0},
      {0, 1},
      {0, 2},
  };
  std::ranges::sort(points, common::argument_sort);
  EXPECT_EQ(points[0], common::point_t(1, 0));
  EXPECT_EQ(points[1], common::point_t(2, 0));
  EXPECT_EQ(points[2], common::point_t(2, 1));
  EXPECT_EQ(points[3], common::point_t(1, 1));
  EXPECT_EQ(points[4], common::point_t(2, 2));
  EXPECT_EQ(points[5], common::point_t(1, 2));
  EXPECT_EQ(points[6], common::point_t(0, 1));
  EXPECT_EQ(points[7], common::point_t(0, 2));
}

TEST(Math, ArgumentSort2) {
  common::point_t points[] = {
      { 1,  0},
      { 1, -1},
      { 0,  1},
      { 0, -1},
      {-1,  1},
      {-1,  0},
      {-1, -1},
      { 1,  1},
  };
  std::ranges::sort(points, common::argument_sort);
  EXPECT_EQ(points[0], common::point_t(1, 0));
  EXPECT_EQ(points[1], common::point_t(1, 1));
  EXPECT_EQ(points[2], common::point_t(0, 1));
  EXPECT_EQ(points[3], common::point_t(-1, 1));
  EXPECT_EQ(points[4], common::point_t(-1, 0));
  EXPECT_EQ(points[5], common::point_t(-1, -1));
  EXPECT_EQ(points[6], common::point_t(0, -1));
  EXPECT_EQ(points[7], common::point_t(1, -1));
}

TEST(Math, MapWithArgumentSort) {
  std::map<common::point_t, int, common::argument_sort_t> map;
  map[{-1, -1}] = 0;
  map[{-1, 0}] = 1;
  map[{-1, 1}] = 2;
  map[{0, -1}] = 3;
  map[{0, 1}] = 4;
  map[{1, -1}] = 5;
  map[{1, 0}] = 6;
  map[{1, 1}] = 7;
  EXPECT_EQ(map.size(), 8);
  EXPECT_EQ(map.at({-1, -1}), 0);
  EXPECT_EQ(map.at({-1, 0}), 1);
  EXPECT_EQ(map.at({-1, 1}), 2);
  EXPECT_EQ(map.at({0, -1}), 3);
  EXPECT_EQ(map.at({0, 1}), 4);
  EXPECT_EQ(map.at({1, -1}), 5);
  EXPECT_EQ(map.at({1, 0}), 6);
  EXPECT_EQ(map.at({1, 1}), 7);
}
