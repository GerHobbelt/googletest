// Demo GoogleTest cases
// Build example:
//   mkdir -p build && cd build
//   g++ -std=c++11 -isystem ../include -pthread \
//     ../googletest/samples/demo_demo_test.cc ../src/gtest_main.cc ../src/gtest-all.cc -o demo_test
//   ./demo_test

#include "gtest/gtest.h"
#include <vector>

// Simple tests using EXPECT/ASSERT
TEST(MathTest, AddAndCompare) {
  EXPECT_EQ(2 + 2, 4);
  EXPECT_NE(2 + 2, 5);
  ASSERT_LT(1 + 1, 3);  // ASSERT_* stops this test on failure
}

// Test fixture example
class VectorFixture : public ::testing::Test {
 protected:
  void SetUp() override { v = {1, 2, 3}; }
  std::vector<int> v;
};

TEST_F(VectorFixture, InitialSize) {
  EXPECT_EQ(v.size(), 3u);
}

TEST_F(VectorFixture, PushBackWorks) {
  v.push_back(42);
  EXPECT_EQ(v.back(), 42);
  EXPECT_EQ(v.size(), 4u);
}

// Parameterized test example
class IsEvenTest : public ::testing::TestWithParam<int> {};

TEST_P(IsEvenTest, CheckEven) {
  int n = GetParam();
  EXPECT_EQ(n % 2, 0);
}

INSTANTIATE_TEST_SUITE_P(Evens, IsEvenTest, ::testing::Values(0, 2, 4, 6, 8));

// Small demonstration of using EXPECT_THROW
int Div(int a, int b) {
  if (b == 0) throw std::invalid_argument("division by zero");
  return a / b;
}

TEST(ExceptionTest, DivByZero) {
  EXPECT_THROW(Div(1, 0), std::invalid_argument);
}

// main is provided by gtest_main.cc, so no need to define it here when linking with gtest_main.
