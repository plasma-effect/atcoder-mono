#include "competitive/heuristic/time_control.hpp"
#include "test_utils.hpp"
#include <gtest/gtest.h>
using namespace std::chrono_literals;

TEST(HeuristicTimeControl, OperatorBool) {
  heuristic::time_control_t<100> control;
  EXPECT_TRUE(static_cast<bool>(control));
  std::this_thread::sleep_for(100ms);
  EXPECT_FALSE(static_cast<bool>(control));
}

TEST(HeuristicTimeControl, Frequency1) {
  heuristic::time_control_t<100> control;
  EXPECT_TRUE(static_cast<bool>(control));
  std::this_thread::sleep_for(100ms);
  EXPECT_FALSE(static_cast<bool>(control));
  EXPECT_FALSE(static_cast<bool>(control));
}

TEST(HeuristicTimeControl, Frequency2) {
  heuristic::time_control_t<100, 2> control;
  EXPECT_TRUE(static_cast<bool>(control));
  EXPECT_TRUE(static_cast<bool>(control));
  std::this_thread::sleep_for(100ms);
  EXPECT_TRUE(static_cast<bool>(control));
  EXPECT_FALSE(static_cast<bool>(control));
  EXPECT_FALSE(static_cast<bool>(control));
}

TEST(HeuristicTimeControl, TotalUpdateCount1) {
  heuristic::time_control_t<100> control;
  static_cast<bool>(control);
  static_cast<bool>(control);
  static_cast<bool>(control);
  EXPECT_EQ(control.total_update_count(), 3);
}

TEST(HeuristicTimeControl, TotalUpdateCount2) {
  heuristic::time_control_t<100, 2> control;
  static_cast<bool>(control);
  static_cast<bool>(control);
  static_cast<bool>(control);
  static_cast<bool>(control);
  static_cast<bool>(control);
  EXPECT_EQ(control.total_update_count(), 5);
}

TEST(HeuristicTimeControlAnnealing, TransitionCheck) {
  heuristic::time_control_with_annealing<100, 100, 50> control;
  std::array<int, 3> count{};
  count[0] = control.transition_check(1.0);
  for (int c = 0; c < 10000; ++c) {
    if (control.transition_check(-50)) {
      ++count[1];
    } else {
      ++count[2];
    }
  }
  EXPECT_EQ(count[0], 1);
  EXPECT_NE(count[1], 0);
  EXPECT_NE(count[2], 0);
}

TEST(HeuristicTimeControlAnnealing, TransitionSuccessDecrease) {
  heuristic::time_control_with_annealing<100, 100, 50> control;
  auto start = std::chrono::system_clock::now();
  std::array<int, 3> count{};
  for (int i = 0; i < 3 && control; ++i) {
    for (int c = 0; c < 10000; ++c) {
      if (control.transition_check(-75)) {
        ++count[i];
      }
    }
    std::this_thread::sleep_until(start + (i + 1) * 25ms);
  }
  EXPECT_GT(count[0], count[1]);
  EXPECT_GT(count[1], count[2]);
  EXPECT_NE(count[2], 0);
}
