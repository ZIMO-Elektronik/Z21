#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <z21/z21.hpp>

TEST(Accessory, is_ext_accessory) {
  {
    z21::TurnoutInfo turnout{};
    EXPECT_FALSE(z21::is_ext_accessory(turnout));
  }

  {
    z21::AccessoryInfo accessory{};
    EXPECT_FALSE(z21::is_ext_accessory(accessory));
  }

  {
    z21::AccessoryInfo accessory{.status = z21::AccessoryInfo::Status::Valid};
    EXPECT_TRUE(z21::is_ext_accessory(accessory));
  }
}
