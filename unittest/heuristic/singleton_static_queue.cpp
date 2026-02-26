#include "competitive/heuristic/singleton_static_queue.hpp"
#include <gtest/gtest.h>

using static_queue = heuristic::singleton_static_queue<int, 3>;

TEST(HeuristicSingletonStaticQueue, General) {
  static_queue queue;
  queue.push_back(1);
  queue.push_back(2);
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), 1);
  EXPECT_EQ(queue.size(), 1);

  queue.push_front(3);
  queue.push_front(4);
  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.pop(), 4);
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), 3);
  EXPECT_EQ(queue.size(), 1);

  queue.push_back(5);
  queue.push_back(6);
  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.pop(), 2);
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), 5);
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), 6);
  EXPECT_EQ(queue.size(), 0);
}

TEST(HeuristicSingletonStaticQueue, CapacityOver) {
  static_queue queue;
  queue.push_back(0);
  queue.push_back(0);
  queue.push_back(0);
  EXPECT_DEATH({ queue.push_back(0); }, ".");
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
  queue.push_back(std::pair(0x00, 0x01));
  queue.push_back(std::pair(0x10, 0x11));
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), std::pair(0x00, 0x01));
  EXPECT_EQ(queue.size(), 1);

  queue.push_front(std::pair(0x20, 0x21));
  queue.push_front(std::pair(0x30, 0x31));
  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.pop(), std::pair(0x30, 0x31));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::pair(0x20, 0x21));
  EXPECT_EQ(queue.size(), 1);

  queue.push_back(std::pair(0x40, 0x41));
  queue.push_back(std::pair(0x50, 0x51));
  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.pop(), std::pair(0x10, 0x11));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::pair(0x40, 0x41));
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), std::pair(0x50, 0x51));
  EXPECT_EQ(queue.size(), 0);
}

TEST(HeuristicSingletonStaticQueue, Tuple) {
  heuristic::singleton_static_queue<std::tuple<int, int, int>, 3> queue;
  queue.push_back(std::tuple(0x00, 0x01, 0x02));
  queue.push_back(std::tuple(0x10, 0x11, 0x12));
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), std::tuple(0x00, 0x01, 0x02));
  EXPECT_EQ(queue.size(), 1);

  queue.push_front(std::tuple(0x20, 0x21, 0x22));
  queue.push_front(std::tuple(0x30, 0x31, 0x32));
  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.pop(), std::tuple(0x30, 0x31, 0x32));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::tuple(0x20, 0x21, 0x22));
  EXPECT_EQ(queue.size(), 1);

  queue.push_back(std::tuple(0x40, 0x41, 0x42));
  queue.push_back(std::tuple(0x50, 0x51, 0x52));
  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.pop(), std::tuple(0x10, 0x11, 0x12));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::tuple(0x40, 0x41, 0x42));
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), std::tuple(0x50, 0x51, 0x52));
  EXPECT_EQ(queue.size(), 0);
}

TEST(HeuristicSingletonStaticQueue, PairCustomPush) {
  heuristic::singleton_static_queue<std::pair<int, int>, 3> queue;
  queue.push_back(0x00, 0x01);
  queue.push_back(0x10, 0x11);
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), std::pair(0x00, 0x01));
  EXPECT_EQ(queue.size(), 1);

  queue.push_front(0x20, 0x21);
  queue.push_front(0x30, 0x31);
  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.pop(), std::pair(0x30, 0x31));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::pair(0x20, 0x21));
  EXPECT_EQ(queue.size(), 1);

  queue.push_back(0x40, 0x41);
  queue.push_back(0x50, 0x51);
  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.pop(), std::pair(0x10, 0x11));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::pair(0x40, 0x41));
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), std::pair(0x50, 0x51));
  EXPECT_EQ(queue.size(), 0);
}

TEST(HeuristicSingletonStaticQueue, TupleCustomPush) {
  heuristic::singleton_static_queue<std::tuple<int, int, int>, 3> queue;
  queue.push_back(0x00, 0x01, 0x02);
  queue.push_back(0x10, 0x11, 0x12);
  EXPECT_EQ(queue.size(), 2);

  EXPECT_EQ(queue.pop(), std::tuple(0x00, 0x01, 0x02));
  EXPECT_EQ(queue.size(), 1);

  queue.push_front(0x20, 0x21, 0x22);
  queue.push_front(0x30, 0x31, 0x32);
  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.pop(), std::tuple(0x30, 0x31, 0x32));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::tuple(0x20, 0x21, 0x22));
  EXPECT_EQ(queue.size(), 1);

  queue.push_back(0x40, 0x41, 0x42);
  queue.push_back(0x50, 0x51, 0x52);
  EXPECT_EQ(queue.size(), 3);
  EXPECT_EQ(queue.pop(), std::tuple(0x10, 0x11, 0x12));
  EXPECT_EQ(queue.size(), 2);
  EXPECT_EQ(queue.pop(), std::tuple(0x40, 0x41, 0x42));
  EXPECT_EQ(queue.size(), 1);
  EXPECT_EQ(queue.pop(), std::tuple(0x50, 0x51, 0x52));
  EXPECT_EQ(queue.size(), 0);
}
