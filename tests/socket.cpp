#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <z21/z21.hpp>

TEST(Socket, empty) { EXPECT_FALSE(z21::Socket{}); }