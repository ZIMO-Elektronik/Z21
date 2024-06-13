#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <z21/z21.hpp>

TEST(SpeedSteps, decode_dcc14) {
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'0001u), -1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'0000u), 0);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'0010u), 1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'0011u), 2);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'0100u), 3);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'0101u), 4);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'0110u), 5);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'0111u), 6);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'1000u), 7);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'1001u), 8);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'1010u), 9);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'1011u), 10);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'1100u), 11);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'1101u), 12);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'1110u), 13);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b0000'1111u), 14);

  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'0001u), -1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'0000u), 0);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'0010u), 1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'0011u), 2);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'0100u), 3);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'0101u), 4);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'0110u), 5);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'0111u), 6);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'1000u), 7);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'1001u), 8);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'1010u), 9);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'1011u), 10);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'1100u), 11);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'1101u), 12);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'1110u), 13);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC14, 0b1000'1111u), 14);
}

TEST(SpeedSteps, encode_dcc14) {
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, -1),
            0b0000'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 0), 0b0000'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 1), 0b0000'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 2), 0b0000'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 3), 0b0000'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 4), 0b0000'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 5), 0b0000'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 6), 0b0000'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 7), 0b0000'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 8), 0b0000'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 9), 0b0000'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 10),
            0b0000'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 11),
            0b0000'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 12),
            0b0000'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 13),
            0b0000'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, false, 14),
            0b0000'1111u);

  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, -1), 0b1000'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 0), 0b1000'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 1), 0b1000'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 2), 0b1000'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 3), 0b1000'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 4), 0b1000'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 5), 0b1000'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 6), 0b1000'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 7), 0b1000'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 8), 0b1000'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 9), 0b1000'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 10), 0b1000'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 11), 0b1000'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 12), 0b1000'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 13), 0b1000'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC14, true, 14), 0b1000'1111u);
}

TEST(SpeedSteps, decode_dcc28) {
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'0001u), -1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'0001u), -1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'0000u), 0);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'0000u), 0);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'0010u), 1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'0010u), 2);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'0011u), 3);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'0011u), 4);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'0100u), 5);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'0100u), 6);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'0101u), 7);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'0101u), 8);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'0110u), 9);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'0110u), 10);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'0111u), 11);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'0111u), 12);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'1000u), 13);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'1000u), 14);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'1001u), 15);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'1001u), 16);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'1010u), 17);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'1010u), 18);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'1011u), 19);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'1011u), 20);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'1100u), 21);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'1100u), 22);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'1101u), 23);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'1101u), 24);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'1110u), 25);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'1110u), 26);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0000'1111u), 27);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b0001'1111u), 28);

  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'0001u), -1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'0001u), -1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'0000u), 0);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'0000u), 0);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'0010u), 1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'0010u), 2);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'0011u), 3);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'0011u), 4);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'0100u), 5);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'0100u), 6);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'0101u), 7);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'0101u), 8);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'0110u), 9);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'0110u), 10);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'0111u), 11);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'0111u), 12);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'1000u), 13);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'1000u), 14);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'1001u), 15);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'1001u), 16);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'1010u), 17);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'1010u), 18);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'1011u), 19);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'1011u), 20);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'1100u), 21);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'1100u), 22);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'1101u), 23);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'1101u), 24);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'1110u), 25);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'1110u), 26);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1000'1111u), 27);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC28, 0b1001'1111u), 28);
}

TEST(SpeedSteps, encode_dcc28) {
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, -1),
            0b0000'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 0), 0b0000'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 1), 0b0000'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 2), 0b0001'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 3), 0b0000'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 4), 0b0001'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 5), 0b0000'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 6), 0b0001'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 7), 0b0000'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 8), 0b0001'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 9), 0b0000'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 10),
            0b0001'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 11),
            0b0000'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 12),
            0b0001'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 13),
            0b0000'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 14),
            0b0001'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 15),
            0b0000'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 16),
            0b0001'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 17),
            0b0000'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 18),
            0b0001'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 19),
            0b0000'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 20),
            0b0001'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 21),
            0b0000'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 22),
            0b0001'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 23),
            0b0000'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 24),
            0b0001'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 25),
            0b0000'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 26),
            0b0001'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 27),
            0b0000'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, false, 28),
            0b0001'1111u);

  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, -1), 0b1000'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 0), 0b1000'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 1), 0b1000'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 2), 0b1001'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 3), 0b1000'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 4), 0b1001'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 5), 0b1000'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 6), 0b1001'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 7), 0b1000'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 8), 0b1001'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 9), 0b1000'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 10), 0b1001'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 11), 0b1000'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 12), 0b1001'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 13), 0b1000'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 14), 0b1001'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 15), 0b1000'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 16), 0b1001'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 17), 0b1000'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 18), 0b1001'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 19), 0b1000'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 20), 0b1001'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 21), 0b1000'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 22), 0b1001'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 23), 0b1000'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 24), 0b1001'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 25), 0b1000'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 26), 0b1001'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 27), 0b1000'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC28, true, 28), 0b1001'1111u);
}

TEST(SpeedSteps, decode_dcc128) {
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'0001u), -1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'0000u), 0);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'0010u), 1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'0011u), 2);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'0100u), 3);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'0101u), 4);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'0110u), 5);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'0111u), 6);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'1000u), 7);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'1001u), 8);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'1010u), 9);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'1011u), 10);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'1100u), 11);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'1101u), 12);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'1110u), 13);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0000'1111u), 14);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'0000u), 15);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'0001u), 16);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'0010u), 17);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'0011u), 18);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'0100u), 19);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'0101u), 20);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'0110u), 21);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'0111u), 22);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'1000u), 23);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'1001u), 24);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'1010u), 25);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'1011u), 26);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'1100u), 27);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'1101u), 28);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'1110u), 29);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0001'1111u), 30);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'0000u), 31);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'0001u), 32);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'0010u), 33);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'0011u), 34);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'0100u), 35);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'0101u), 36);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'0110u), 37);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'0111u), 38);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'1000u), 39);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'1001u), 40);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'1010u), 41);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'1011u), 42);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'1100u), 43);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'1101u), 44);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'1110u), 45);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0010'1111u), 46);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'0000u), 47);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'0001u), 48);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'0010u), 49);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'0011u), 50);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'0100u), 51);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'0101u), 52);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'0110u), 53);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'0111u), 54);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'1000u), 55);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'1001u), 56);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'1010u), 57);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'1011u), 58);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'1100u), 59);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'1101u), 60);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'1110u), 61);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0011'1111u), 62);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'0000u), 63);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'0001u), 64);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'0010u), 65);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'0011u), 66);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'0100u), 67);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'0101u), 68);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'0110u), 69);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'0111u), 70);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'1000u), 71);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'1001u), 72);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'1010u), 73);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'1011u), 74);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'1100u), 75);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'1101u), 76);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'1110u), 77);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0100'1111u), 78);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'0000u), 79);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'0001u), 80);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'0010u), 81);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'0011u), 82);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'0100u), 83);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'0101u), 84);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'0110u), 85);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'0111u), 86);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'1000u), 87);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'1001u), 88);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'1010u), 89);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'1011u), 90);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'1100u), 91);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'1101u), 92);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'1110u), 93);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0101'1111u), 94);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'0000u), 95);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'0001u), 96);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'0010u), 97);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'0011u), 98);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'0100u), 99);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'0101u), 100);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'0110u), 101);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'0111u), 102);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'1000u), 103);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'1001u), 104);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'1010u), 105);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'1011u), 106);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'1100u), 107);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'1101u), 108);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'1110u), 109);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0110'1111u), 110);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'0000u), 111);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'0001u), 112);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'0010u), 113);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'0011u), 114);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'0100u), 115);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'0101u), 116);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'0110u), 117);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'0111u), 118);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'1000u), 119);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'1001u), 120);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'1010u), 121);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'1011u), 122);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'1100u), 123);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'1101u), 124);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'1110u), 125);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b0111'1111u), 126);

  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'0001u), -1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'0000u), 0);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'0010u), 1);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'0011u), 2);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'0100u), 3);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'0101u), 4);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'0110u), 5);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'0111u), 6);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'1000u), 7);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'1001u), 8);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'1010u), 9);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'1011u), 10);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'1100u), 11);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'1101u), 12);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'1110u), 13);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1000'1111u), 14);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'0000u), 15);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'0001u), 16);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'0010u), 17);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'0011u), 18);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'0100u), 19);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'0101u), 20);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'0110u), 21);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'0111u), 22);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'1000u), 23);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'1001u), 24);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'1010u), 25);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'1011u), 26);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'1100u), 27);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'1101u), 28);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'1110u), 29);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1001'1111u), 30);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'0000u), 31);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'0001u), 32);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'0010u), 33);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'0011u), 34);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'0100u), 35);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'0101u), 36);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'0110u), 37);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'0111u), 38);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'1000u), 39);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'1001u), 40);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'1010u), 41);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'1011u), 42);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'1100u), 43);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'1101u), 44);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'1110u), 45);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1010'1111u), 46);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'0000u), 47);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'0001u), 48);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'0010u), 49);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'0011u), 50);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'0100u), 51);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'0101u), 52);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'0110u), 53);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'0111u), 54);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'1000u), 55);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'1001u), 56);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'1010u), 57);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'1011u), 58);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'1100u), 59);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'1101u), 60);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'1110u), 61);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1011'1111u), 62);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'0000u), 63);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'0001u), 64);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'0010u), 65);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'0011u), 66);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'0100u), 67);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'0101u), 68);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'0110u), 69);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'0111u), 70);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'1000u), 71);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'1001u), 72);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'1010u), 73);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'1011u), 74);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'1100u), 75);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'1101u), 76);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'1110u), 77);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1100'1111u), 78);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'0000u), 79);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'0001u), 80);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'0010u), 81);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'0011u), 82);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'0100u), 83);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'0101u), 84);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'0110u), 85);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'0111u), 86);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'1000u), 87);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'1001u), 88);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'1010u), 89);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'1011u), 90);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'1100u), 91);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'1101u), 92);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'1110u), 93);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1101'1111u), 94);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'0000u), 95);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'0001u), 96);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'0010u), 97);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'0011u), 98);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'0100u), 99);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'0101u), 100);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'0110u), 101);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'0111u), 102);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'1000u), 103);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'1001u), 104);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'1010u), 105);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'1011u), 106);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'1100u), 107);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'1101u), 108);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'1110u), 109);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1110'1111u), 110);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'0000u), 111);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'0001u), 112);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'0010u), 113);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'0011u), 114);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'0100u), 115);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'0101u), 116);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'0110u), 117);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'0111u), 118);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'1000u), 119);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'1001u), 120);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'1010u), 121);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'1011u), 122);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'1100u), 123);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'1101u), 124);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'1110u), 125);
  EXPECT_EQ(z21::decode_rvvvvvvv(z21::LocoInfo::DCC128, 0b1111'1111u), 126);
}

TEST(SpeedSteps, encode_dcc128) {
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, -1),
            0b0000'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 0),
            0b0000'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 1),
            0b0000'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 2),
            0b0000'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 3),
            0b0000'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 4),
            0b0000'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 5),
            0b0000'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 6),
            0b0000'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 7),
            0b0000'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 8),
            0b0000'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 9),
            0b0000'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 10),
            0b0000'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 11),
            0b0000'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 12),
            0b0000'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 13),
            0b0000'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 14),
            0b0000'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 15),
            0b0001'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 16),
            0b0001'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 17),
            0b0001'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 18),
            0b0001'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 19),
            0b0001'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 20),
            0b0001'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 21),
            0b0001'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 22),
            0b0001'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 23),
            0b0001'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 24),
            0b0001'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 25),
            0b0001'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 26),
            0b0001'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 27),
            0b0001'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 28),
            0b0001'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 29),
            0b0001'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 30),
            0b0001'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 31),
            0b0010'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 32),
            0b0010'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 33),
            0b0010'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 34),
            0b0010'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 35),
            0b0010'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 36),
            0b0010'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 37),
            0b0010'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 38),
            0b0010'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 39),
            0b0010'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 40),
            0b0010'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 41),
            0b0010'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 42),
            0b0010'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 43),
            0b0010'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 44),
            0b0010'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 45),
            0b0010'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 46),
            0b0010'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 47),
            0b0011'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 48),
            0b0011'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 49),
            0b0011'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 50),
            0b0011'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 51),
            0b0011'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 52),
            0b0011'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 53),
            0b0011'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 54),
            0b0011'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 55),
            0b0011'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 56),
            0b0011'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 57),
            0b0011'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 58),
            0b0011'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 59),
            0b0011'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 60),
            0b0011'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 61),
            0b0011'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 62),
            0b0011'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 63),
            0b0100'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 64),
            0b0100'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 65),
            0b0100'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 66),
            0b0100'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 67),
            0b0100'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 68),
            0b0100'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 69),
            0b0100'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 70),
            0b0100'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 71),
            0b0100'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 72),
            0b0100'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 73),
            0b0100'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 74),
            0b0100'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 75),
            0b0100'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 76),
            0b0100'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 77),
            0b0100'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 78),
            0b0100'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 79),
            0b0101'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 80),
            0b0101'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 81),
            0b0101'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 82),
            0b0101'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 83),
            0b0101'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 84),
            0b0101'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 85),
            0b0101'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 86),
            0b0101'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 87),
            0b0101'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 88),
            0b0101'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 89),
            0b0101'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 90),
            0b0101'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 91),
            0b0101'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 92),
            0b0101'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 93),
            0b0101'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 94),
            0b0101'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 95),
            0b0110'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 96),
            0b0110'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 97),
            0b0110'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 98),
            0b0110'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 99),
            0b0110'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 100),
            0b0110'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 101),
            0b0110'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 102),
            0b0110'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 103),
            0b0110'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 104),
            0b0110'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 105),
            0b0110'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 106),
            0b0110'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 107),
            0b0110'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 108),
            0b0110'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 109),
            0b0110'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 110),
            0b0110'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 111),
            0b0111'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 112),
            0b0111'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 113),
            0b0111'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 114),
            0b0111'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 115),
            0b0111'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 116),
            0b0111'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 117),
            0b0111'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 118),
            0b0111'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 119),
            0b0111'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 120),
            0b0111'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 121),
            0b0111'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 122),
            0b0111'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 123),
            0b0111'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 124),
            0b0111'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 125),
            0b0111'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, false, 126),
            0b0111'1111u);

  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, -1),
            0b1000'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 0), 0b1000'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 1), 0b1000'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 2), 0b1000'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 3), 0b1000'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 4), 0b1000'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 5), 0b1000'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 6), 0b1000'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 7), 0b1000'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 8), 0b1000'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 9), 0b1000'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 10),
            0b1000'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 11),
            0b1000'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 12),
            0b1000'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 13),
            0b1000'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 14),
            0b1000'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 15),
            0b1001'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 16),
            0b1001'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 17),
            0b1001'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 18),
            0b1001'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 19),
            0b1001'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 20),
            0b1001'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 21),
            0b1001'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 22),
            0b1001'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 23),
            0b1001'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 24),
            0b1001'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 25),
            0b1001'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 26),
            0b1001'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 27),
            0b1001'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 28),
            0b1001'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 29),
            0b1001'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 30),
            0b1001'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 31),
            0b1010'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 32),
            0b1010'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 33),
            0b1010'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 34),
            0b1010'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 35),
            0b1010'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 36),
            0b1010'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 37),
            0b1010'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 38),
            0b1010'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 39),
            0b1010'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 40),
            0b1010'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 41),
            0b1010'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 42),
            0b1010'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 43),
            0b1010'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 44),
            0b1010'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 45),
            0b1010'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 46),
            0b1010'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 47),
            0b1011'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 48),
            0b1011'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 49),
            0b1011'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 50),
            0b1011'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 51),
            0b1011'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 52),
            0b1011'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 53),
            0b1011'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 54),
            0b1011'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 55),
            0b1011'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 56),
            0b1011'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 57),
            0b1011'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 58),
            0b1011'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 59),
            0b1011'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 60),
            0b1011'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 61),
            0b1011'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 62),
            0b1011'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 63),
            0b1100'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 64),
            0b1100'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 65),
            0b1100'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 66),
            0b1100'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 67),
            0b1100'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 68),
            0b1100'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 69),
            0b1100'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 70),
            0b1100'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 71),
            0b1100'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 72),
            0b1100'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 73),
            0b1100'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 74),
            0b1100'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 75),
            0b1100'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 76),
            0b1100'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 77),
            0b1100'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 78),
            0b1100'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 79),
            0b1101'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 80),
            0b1101'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 81),
            0b1101'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 82),
            0b1101'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 83),
            0b1101'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 84),
            0b1101'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 85),
            0b1101'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 86),
            0b1101'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 87),
            0b1101'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 88),
            0b1101'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 89),
            0b1101'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 90),
            0b1101'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 91),
            0b1101'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 92),
            0b1101'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 93),
            0b1101'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 94),
            0b1101'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 95),
            0b1110'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 96),
            0b1110'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 97),
            0b1110'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 98),
            0b1110'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 99),
            0b1110'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 100),
            0b1110'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 101),
            0b1110'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 102),
            0b1110'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 103),
            0b1110'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 104),
            0b1110'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 105),
            0b1110'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 106),
            0b1110'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 107),
            0b1110'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 108),
            0b1110'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 109),
            0b1110'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 110),
            0b1110'1111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 111),
            0b1111'0000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 112),
            0b1111'0001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 113),
            0b1111'0010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 114),
            0b1111'0011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 115),
            0b1111'0100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 116),
            0b1111'0101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 117),
            0b1111'0110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 118),
            0b1111'0111u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 119),
            0b1111'1000u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 120),
            0b1111'1001u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 121),
            0b1111'1010u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 122),
            0b1111'1011u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 123),
            0b1111'1100u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 124),
            0b1111'1101u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 125),
            0b1111'1110u);
  EXPECT_EQ(z21::encode_rvvvvvvv(z21::LocoInfo::DCC128, true, 126),
            0b1111'1111u);
}