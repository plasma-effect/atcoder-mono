#include "competitive/container/static_queue.hpp"
#include "test_utils.hpp"
#include <gtest/gtest.h>

TEST(StaticQueue, General) {
  competitive::static_queue<int, 3> queue;
  queue.push(1);
  queue.push(2);
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), 1);
  EXPECT_EQ(queue.size(), 1);

  queue.push(3);
  queue.push(4);
  EXPECT_EQ(queue.size(), 3);

  EXPECT_EQ(queue.pop(), 2);
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), 3);
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), 4);
  EXPECT_EQ(queue.size(), 0);
}

TEST(StaticQueue, Pair) {
  competitive::static_queue<std::pair<int, int>, 3> queue;
  queue.push(std::pair(1, 2));
  queue.push(std::pair(3, 4));
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), std::pair(1, 2));
  EXPECT_EQ(queue.size(), 1);

  queue.push(std::pair(5, 6));
  queue.push(std::pair(7, 8));
  EXPECT_EQ(queue.size(), 3);

  EXPECT_EQ(queue.pop(), std::pair(3, 4));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::pair(5, 6));
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), std::pair(7, 8));
  EXPECT_EQ(queue.size(), 0);
}

TEST(StaticQueue, Tuple) {
  competitive::static_queue<std::tuple<int, int, int>, 3> queue;
  queue.push(std::tuple(0x00, 0x01, 0x02));
  queue.push(std::tuple(0x10, 0x11, 0x12));
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), std::tuple(0x00, 0x01, 0x02));
  EXPECT_EQ(queue.size(), 1);

  queue.push(std::tuple(0x20, 0x21, 0x22));
  queue.push(std::tuple(0x30, 0x31, 0x32));
  EXPECT_EQ(queue.size(), 3);

  EXPECT_EQ(queue.pop(), std::tuple(0x10, 0x11, 0x12));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::tuple(0x20, 0x21, 0x22));
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), std::tuple(0x30, 0x31, 0x32));
  EXPECT_EQ(queue.size(), 0);
}

TEST(StaticQueue, PairUnCopyable) {
  using uncopyable = test_utils::uncopyable<int>;
  competitive::static_queue<std::pair<int, uncopyable>, 3> queue;
  queue.push(1, 2);
  queue.push(3, uncopyable(4));
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), std::pair(1, uncopyable(2)));
  EXPECT_EQ(queue.size(), 1);

  queue.push(5, 6);
  queue.push(7, 8);
  EXPECT_EQ(queue.size(), 3);

  EXPECT_EQ(queue.pop(), std::pair(3, uncopyable(4)));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::pair(5, uncopyable(6)));
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), std::pair(7, uncopyable(8)));
  EXPECT_EQ(queue.size(), 0);
}

TEST(StaticQueue, TupleUnCopyable) {
  using uncopyable = test_utils::uncopyable<int>;
  competitive::static_queue<std::tuple<int, uncopyable, int>, 3> queue;
  queue.push(0x00, 0x01, 0x02);
  queue.push(0x10, uncopyable(0x11), 0x12);
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), std::tuple(0x00, uncopyable(0x01), 0x02));
  EXPECT_EQ(queue.size(), 1);

  queue.push(0x20, 0x21, 0x22);
  queue.push(0x30, 0x31, 0x32);
  EXPECT_EQ(queue.size(), 3);

  EXPECT_EQ(queue.pop(), std::tuple(0x10, uncopyable(0x11), 0x12));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::tuple(0x20, uncopyable(0x21), 0x22));
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), std::tuple(0x30, uncopyable(0x31), 0x32));
  EXPECT_EQ(queue.size(), 0);
}

TEST(StaticQueue, PairCustomPush) {
  competitive::static_queue<std::pair<int, int>, 3> queue;
  queue.push(1, 2);
  queue.push(3, 4);
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), std::pair(1, 2));
  EXPECT_EQ(queue.size(), 1);

  queue.push(5, 6);
  queue.push(7, 8);
  EXPECT_EQ(queue.size(), 3);

  EXPECT_EQ(queue.pop(), std::pair(3, 4));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::pair(5, 6));
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), std::pair(7, 8));
  EXPECT_EQ(queue.size(), 0);
}

TEST(StaticQueue, TupleCustomPush) {
  competitive::static_queue<std::tuple<int, int, int>, 3> queue;
  queue.push(0x00, 0x01, 0x02);
  queue.push(0x10, 0x11, 0x12);
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), std::tuple(0x00, 0x01, 0x02));
  EXPECT_EQ(queue.size(), 1);

  queue.push(0x20, 0x21, 0x22);
  queue.push(0x30, 0x31, 0x32);
  EXPECT_EQ(queue.size(), 3);

  EXPECT_EQ(queue.pop(), std::tuple(0x10, 0x11, 0x12));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::tuple(0x20, 0x21, 0x22));
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), std::tuple(0x30, 0x31, 0x32));
  EXPECT_EQ(queue.size(), 0);
}
