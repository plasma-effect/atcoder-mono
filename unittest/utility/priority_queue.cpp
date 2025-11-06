#include "competitive/utility/priority_queue.hpp"
#include <gtest/gtest.h>

TEST(Utility, PriorityQueueDefault) {
  common::priority_queue<int> queue;
  for (auto v : {1, 3, 2, 4}) {
    queue.emplace(v);
  }
  {
    auto v = queue.top();
    queue.pop();
    ASSERT_EQ(v, 1);
  }
  {
    auto v = queue.top();
    queue.pop();
    ASSERT_EQ(v, 2);
  }
  {
    auto v = queue.top();
    queue.pop();
    ASSERT_EQ(v, 3);
  }
  {
    auto v = queue.top();
    queue.pop();
    ASSERT_EQ(v, 4);
  }
}
