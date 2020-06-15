#include "../include/helloWorld.h"
#include <gtest/gtest.h>

class DefaultTestSuite : public ::testing::Test {
protected:
  DefaultTestSuite() : helloWorld() {}
  HelloWorld helloWorld;
};

TEST_F(DefaultTestSuite, TestSquare) {
  EXPECT_EQ(helloWorld.Square(4), 16);
  EXPECT_EQ(helloWorld.Square(5), 25);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}