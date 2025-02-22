#include "gtest/gtest.h"

extern "C" {
#include "files_server.h"
}

TEST(FirstTest, IntegerTest) {
    EXPECT_EQ(5, test_gtest());
}