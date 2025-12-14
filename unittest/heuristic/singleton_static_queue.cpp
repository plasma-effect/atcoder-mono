#include "competitive/heuristic/singleton_static_queue.hpp"
#include <gtest/gtest.h>

using static_queue = heuristic::singleton_static_queue<int, 3>;

TEST(HeuristicSingletonStaticQueue, General) {
  static_queue queue;
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

TEST(HeuristicSingletonStaticQueue, CapacityOver) {
  static_queue queue;
  queue.push(0);
  queue.push(0);
  queue.push(0);
  EXPECT_DEATH({ queue.push(0); }, ".");
}

TEST(HeuristicSingletonStaticQueue, NullPop) {
  static_queue queue;
  EXPECT_DEATH({ queue.pop(); }, ".");
}

#ifdef LOCAL_DEBUG
TEST(HeuristicSingletonStaticQueue, DualDefined) {
  static_queue queue1;
  EXPECT_DEATH({ static_queue queue2; }, ".");
}
#endif

TEST(HeuristicSingletonStaticQueue, Pair) {
  heuristic::singleton_static_queue<std::pair<int, int>, 3> queue;
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

TEST(HeuristicSingletonStaticQueue, Tuple) {
  heuristic::singleton_static_queue<std::tuple<int, int, int>, 3> queue;
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

TEST(HeuristicSingletonStaticQueue, PairCustomPush) {
  heuristic::singleton_static_queue<std::pair<int, int>, 3> queue;
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

TEST(HeuristicSingletonStaticQueue, TupleCustomPush) {
  heuristic::singleton_static_queue<std::tuple<int, int, int>, 3> queue;
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
