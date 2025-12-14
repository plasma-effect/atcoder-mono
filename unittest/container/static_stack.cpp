#include "competitive/container/static_stack.hpp"
#include "test_utils.hpp"
#include <gtest/gtest.h>

TEST(StaticStack, General) {
  competitive::static_stack<int, 3> stack;
  stack.push(1);
  stack.push(2);
  EXPECT_EQ(stack.size(), 2);

  EXPECT_EQ(stack.pop(), 2);
  EXPECT_EQ(stack.size(), 1);

  stack.push(3);
  stack.push(4);
  EXPECT_EQ(stack.size(), 3);

  EXPECT_EQ(stack.pop(), 4);
  EXPECT_EQ(stack.size(), 2);
  EXPECT_EQ(stack.pop(), 3);
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.pop(), 1);
  EXPECT_EQ(stack.size(), 0);
}

TEST(StaticStack, Pair) {
  competitive::static_stack<std::pair<int, int>, 3> stack;
  stack.push(std::pair(0x00, 0x01));
  stack.push(std::pair(0x10, 0x11));
  EXPECT_EQ(stack.size(), 2);

  EXPECT_EQ(stack.pop(), std::pair(0x10, 0x11));
  EXPECT_EQ(stack.size(), 1);

  stack.push(std::pair(0x20, 0x21));
  stack.push(std::pair(0x30, 0x31));
  EXPECT_EQ(stack.size(), 3);

  EXPECT_EQ(stack.pop(), std::pair(0x30, 0x31));
  EXPECT_EQ(stack.size(), 2);
  EXPECT_EQ(stack.pop(), std::pair(0x20, 0x21));
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.pop(), std::pair(0x00, 0x01));
  EXPECT_EQ(stack.size(), 0);
}

TEST(StaticStack, Tuple) {
  competitive::static_stack<std::tuple<int, int, int>, 3> stack;
  stack.push(std::tuple(0x00, 0x01, 0x02));
  stack.push(std::tuple(0x10, 0x11, 0x12));
  EXPECT_EQ(stack.size(), 2);

  EXPECT_EQ(stack.pop(), std::tuple(0x10, 0x11, 0x12));
  EXPECT_EQ(stack.size(), 1);

  stack.push(std::tuple(0x20, 0x21, 0x22));
  stack.push(std::tuple(0x30, 0x31, 0x32));
  EXPECT_EQ(stack.size(), 3);

  EXPECT_EQ(stack.pop(), std::tuple(0x30, 0x31, 0x32));
  EXPECT_EQ(stack.size(), 2);
  EXPECT_EQ(stack.pop(), std::tuple(0x20, 0x21, 0x22));
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.pop(), std::tuple(0x00, 0x01, 0x02));
  EXPECT_EQ(stack.size(), 0);
}

TEST(StaticStack, PairUnCopyable) {
  using uncopyable = test_utils::uncopyable<int>;
  competitive::static_stack<std::pair<int, uncopyable>, 3> stack;
  stack.push(0x00, 0x01);
  stack.push(0x10, uncopyable(0x11));
  EXPECT_EQ(stack.size(), 2);

  EXPECT_EQ(stack.pop(), std::pair(0x10, uncopyable(0x11)));
  EXPECT_EQ(stack.size(), 1);

  stack.push(0x20, 0x21);
  stack.push(0x30, 0x31);
  EXPECT_EQ(stack.size(), 3);

  EXPECT_EQ(stack.pop(), std::pair(0x30, uncopyable(0x31)));
  EXPECT_EQ(stack.size(), 2);
  EXPECT_EQ(stack.pop(), std::pair(0x20, uncopyable(0x21)));
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.pop(), std::pair(0x00, uncopyable(0x01)));
  EXPECT_EQ(stack.size(), 0);
}

TEST(StaticStack, TupleUnCopyable) {
  using uncopyable = test_utils::uncopyable<int>;
  competitive::static_stack<std::tuple<int, uncopyable, int>, 3> stack;
  stack.push(0x00, 0x01, 0x02);
  stack.push(0x10, uncopyable(0x11), 0x12);
  EXPECT_EQ(stack.size(), 2);

  EXPECT_EQ(stack.pop(), std::tuple(0x10, uncopyable(0x11), 0x12));
  EXPECT_EQ(stack.size(), 1);

  stack.push(0x20, 0x21, 0x22);
  stack.push(0x30, 0x31, 0x32);
  EXPECT_EQ(stack.size(), 3);

  EXPECT_EQ(stack.pop(), std::tuple(0x30, uncopyable(0x31), 0x32));
  EXPECT_EQ(stack.size(), 2);
  EXPECT_EQ(stack.pop(), std::tuple(0x20, uncopyable(0x21), 0x22));
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.pop(), std::tuple(0x00, uncopyable(0x01), 0x02));
  EXPECT_EQ(stack.size(), 0);
}

TEST(StaticStack, PairCustomPush) {
  competitive::static_stack<std::pair<int, int>, 3> stack;
  stack.push(0x00, 0x01);
  stack.push(0x10, 0x11);
  EXPECT_EQ(stack.size(), 2);

  EXPECT_EQ(stack.pop(), std::pair(0x10, 0x11));
  EXPECT_EQ(stack.size(), 1);

  stack.push(0x20, 0x21);
  stack.push(0x30, 0x31);
  EXPECT_EQ(stack.size(), 3);

  EXPECT_EQ(stack.pop(), std::pair(0x30, 0x31));
  EXPECT_EQ(stack.size(), 2);
  EXPECT_EQ(stack.pop(), std::pair(0x20, 0x21));
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.pop(), std::pair(0x00, 0x01));
  EXPECT_EQ(stack.size(), 0);
}

TEST(StaticStack, TupleCustomPush) {
  competitive::static_stack<std::tuple<int, int, int>, 3> stack;
  stack.push(0x00, 0x01, 0x02);
  stack.push(0x10, 0x11, 0x12);
  EXPECT_EQ(stack.size(), 2);

  EXPECT_EQ(stack.pop(), std::tuple(0x10, 0x11, 0x12));
  EXPECT_EQ(stack.size(), 1);

  stack.push(0x20, 0x21, 0x22);
  stack.push(0x30, 0x31, 0x32);
  EXPECT_EQ(stack.size(), 3);

  EXPECT_EQ(stack.pop(), std::tuple(0x30, 0x31, 0x32));
  EXPECT_EQ(stack.size(), 2);
  EXPECT_EQ(stack.pop(), std::tuple(0x20, 0x21, 0x22));
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.pop(), std::tuple(0x00, 0x01, 0x02));
  EXPECT_EQ(stack.size(), 0);
}
