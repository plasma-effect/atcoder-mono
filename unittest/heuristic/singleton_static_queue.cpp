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
