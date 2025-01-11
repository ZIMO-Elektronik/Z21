#include "base_test.hpp"
#include "utility.hpp"

TEST_F(BaseTest, track_power_off) {
  EXPECT_CALL(_mock, trackPower(false))
    .WillOnce(Return(true))
    .WillOnce(Return(false));

  // Only expect broadcast if trackPower returns true
  EXPECT_CALL(_mock, transmit(_, _)).Times(1);

  Receive(make_lan_x_set_track_power_off_dataset());
  Execute();

  Receive(make_lan_x_set_track_power_off_dataset());
  Execute();
}

TEST_F(BaseTest, track_power_on) {
  EXPECT_CALL(_mock, trackPower(true))
    .WillOnce(Return(true))
    .WillOnce(Return(false));

  // Only expect broadcast if trackPower returns true
  EXPECT_CALL(_mock, transmit(_, _)).Times(1);

  Receive(make_lan_x_set_track_power_on_dataset());
  Execute();

  Receive(make_lan_x_set_track_power_on_dataset());
  Execute();
}