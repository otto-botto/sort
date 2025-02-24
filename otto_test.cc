#include "gtest/gtest.h"

extern "C" {
#include "otto.h"
}

TEST(Otto, HelloTest) {
    EXPECT_EQ(3, hello());
}