#include "competitive/container/static_dual_array.hpp"
#include "competitive/stdio.hpp"
#include <gtest/gtest.h>

TEST(Container, StaticDualArrayAssignAndRead) {
  common::static_dual_array<int, 2, 3> ar;
  ar(0, 0) = 0;
  ar(0, 1) = 1;
  ar(0, 2) = 2;
  ar(1, 0) = 3;
  ar(1, 1) = 4;
  ar(1, 2) = 5;
  ASSERT_EQ(ar(0, 0), 0);
  ASSERT_EQ(ar(0, 1), 1);
  ASSERT_EQ(ar(0, 2), 2);
  ASSERT_EQ(ar(1, 0), 3);
  ASSERT_EQ(ar(1, 1), 4);
  ASSERT_EQ(ar(1, 2), 5);
}

TEST(Container, StaticDualArrayAssignAndReadByAt) {
  common::static_dual_array<int, 2, 3> ar;
  ar.at(0, 0) = 0;
  ar.at(0, 1) = 1;
  ar.at(0, 2) = 2;
  ar.at(1, 0) = 3;
  ar.at(1, 1) = 4;
  ar.at(1, 2) = 5;
  ASSERT_EQ(ar.at(0, 0), 0);
  ASSERT_EQ(ar.at(0, 1), 1);
  ASSERT_EQ(ar.at(0, 2), 2);
  ASSERT_EQ(ar.at(1, 0), 3);
  ASSERT_EQ(ar.at(1, 1), 4);
  ASSERT_EQ(ar.at(1, 2), 5);
}

TEST(Container, StaticDualArrayGetDimensions) {
  common::static_dual_array<int, 2, 3> ar;
  auto [d0, d1] = ar.dimensions();
  ASSERT_EQ(d0, 2u);
  ASSERT_EQ(d1, 3u);
}

TEST(Container, StaticDualArrayGetSize) {
  common::static_dual_array<int, 2, 3> ar;
  ASSERT_EQ(ar.size(), 6u);
}

TEST(Container, StaticDualArrayOutOfBound) {
  common::static_dual_array<int, 2, 3> ar;
  const auto& car = ar;

  ASSERT_DEATH({ ar(2, 4); }, ".");
  ASSERT_DEATH({ ar(2, 0); }, ".");
  ASSERT_DEATH({ ar(2, -1); }, ".");
  ASSERT_DEATH({ ar(0, 4); }, ".");
  ASSERT_DEATH({ ar(0, -1); }, ".");
  ASSERT_DEATH({ ar(-1, 4); }, ".");
  ASSERT_DEATH({ ar(-1, 0); }, ".");
  ASSERT_DEATH({ ar(-1, -1); }, ".");

  ASSERT_DEATH({ car(2, 4); }, ".");
  ASSERT_DEATH({ car(2, 0); }, ".");
  ASSERT_DEATH({ car(2, -1); }, ".");
  ASSERT_DEATH({ car(0, 4); }, ".");
  ASSERT_DEATH({ car(0, -1); }, ".");
  ASSERT_DEATH({ car(-1, 4); }, ".");
  ASSERT_DEATH({ car(-1, 0); }, ".");
  ASSERT_DEATH({ car(-1, -1); }, ".");

  ASSERT_THROW({ ar.at(2, 4); }, std::out_of_range);
  ASSERT_THROW({ ar.at(2, 0); }, std::out_of_range);
  ASSERT_THROW({ ar.at(2, -1); }, std::out_of_range);
  ASSERT_THROW({ ar.at(0, 4); }, std::out_of_range);
  ASSERT_THROW({ ar.at(0, -1); }, std::out_of_range);
  ASSERT_THROW({ ar.at(-1, 4); }, std::out_of_range);
  ASSERT_THROW({ ar.at(-1, 0); }, std::out_of_range);
  ASSERT_THROW({ ar.at(-1, -1); }, std::out_of_range);

  ASSERT_THROW({ car.at(2, 4); }, std::out_of_range);
  ASSERT_THROW({ car.at(2, 0); }, std::out_of_range);
  ASSERT_THROW({ car.at(2, -1); }, std::out_of_range);
  ASSERT_THROW({ car.at(0, 4); }, std::out_of_range);
  ASSERT_THROW({ car.at(0, -1); }, std::out_of_range);
  ASSERT_THROW({ car.at(-1, 4); }, std::out_of_range);
  ASSERT_THROW({ car.at(-1, 0); }, std::out_of_range);
  ASSERT_THROW({ car.at(-1, -1); }, std::out_of_range);
}

TEST(Container, StaticDualArrayDebugPrint) {
  common::static_dual_array<int, 2, 3> ar;
  ar(0, 0) = 0;
  ar(0, 1) = 1;
  ar(0, 2) = 2;
  ar(1, 0) = 3;
  ar(1, 1) = 4;
  ar(1, 2) = 5;
  testing::internal::CaptureStderr();
  common::debug::println(ar);
  EXPECT_EQ(testing::internal::GetCapturedStderr(), "{{0, 1, 2}, {3, 4, 5}}\n");
}

TEST(Container, StaticDualArrayNormalPrint) {
  common::static_dual_array<int, 2, 3> ar;
  ar(0, 0) = 0;
  ar(0, 1) = 1;
  ar(0, 2) = 2;
  ar(1, 0) = 3;
  ar(1, 1) = 4;
  ar(1, 2) = 5;
  testing::internal::CaptureStdout();
  common::println(ar);
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "0 1 2 3 4 5\n");
}
