// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Dataset header
///
/// \file   z21/header.hpp
/// \author Vincent Hamp
/// \date   10/11/2023

#pragma once

#include <cstdint>

namespace z21 {

///
enum class Header : uint8_t {
  // clang-format off
  // Client to Z21
  LAN_GET_SERIAL_NUMBER = 0x10u,
  LAN_GET_COMMON_SETTINGS = 0x12u,
  LAN_SET_COMMON_SETTINGS = 0x13u,
  LAN_GET_MMDCC_SETTINGS = 0x16u,
  LAN_SET_MMDCC_SETTINGS = 0x17u,
  LAN_GET_CODE = 0x18u,
  LAN_GET_HWINFO = 0x1A,
  LAN_LOGOFF = 0x30u,
  LAN_X = 0x40u, LAN_X_GET_VERSION = 0x40u, LAN_X_GET_STATUS = 0x40u, LAN_X_SET_TRACK_POWER_OFF = 0x40u, LAN_X_SET_TRACK_POWER_ON = 0x40u, LAN_X_DCC_READ_REGISTER = 0x40u, LAN_X_CV_READ = 0x40u, LAN_X_DCC_WRITE_REGISTER = 0x40u, LAN_X_CV_WRITE = 0x40u, LAN_X_MM_WRITE_BYTE = 0x40u, LAN_X_GET_TURNOUT_INFO = 0x40u, LAN_X_GET_EXT_ACCESSORY_INFO = 0x40u, LAN_X_SET_TURNOUT = 0x40u, LAN_X_SET_EXT_ACCESSORY = 0x40u, LAN_X_SET_STOP = 0x40u, LAN_X_SET_LOCO_E_STOP = 0x40u, LAN_X_PURGE_LOCO = 0x40u, LAN_X_GET_LOCO_INFO = 0x40u, LAN_X_SET_LOCO_DRIVE = 0x40u, LAN_X_SET_LOCO_FUNCTION = 0x40u, LAN_X_SET_LOCO_FUNCTION_GROUP = 0x40u, LAN_X_SET_LOCO_BINARY_STATE = 0x40u, LAN_X_CV_POM_WRITE_BYTE = 0x40u, LAN_X_CV_POM_WRITE_BIT = 0x40u, LAN_X_CV_POM_READ_BYTE = 0x40u, LAN_X_CV_POM_ACCESSORY_WRITE_BYTE = 0x40u, LAN_X_CV_POM_ACCESSORY_WRITE_BIT = 0x40u, LAN_X_CV_POM_ACCESSORY_READ_BYTE = 0x40u, LAN_X_GET_FIRMWARE_VERSION = 0x40u,
  LAN_SET_BROADCASTFLAGS = 0x50u,
  LAN_GET_BROADCASTFLAGS = 0x51u,
  LAN_GET_LOCOMODE = 0x60u,
  LAN_SET_LOCOMODE = 0x61u,
  LAN_GET_TURNOUTMODE = 0x70u,
  LAN_SET_TURNOUTMODE = 0x71u,
  LAN_RMBUS_GETDATA = 0x81u,
  LAN_RMBUS_PROGRAMMODULE = 0x82u,
  LAN_SYSTEMSTATE_GETDATA = 0x85u,
  LAN_RAILCOM_GETDATA = 0x89u,
  LAN_LOCONET_FROM_LAN = 0xA2u,
  LAN_LOCONET_DISPATCH_ADDR = 0xA3u,
  LAN_LOCONET_DETECTOR = 0xA4u,
  LAN_CAN_DETECTOR = 0xC4u,
  LAN_CAN_DEVICE_GET_DESCRIPTION = 0xC8u,
  LAN_CAN_DEVICE_SET_DESCRIPTION = 0xC9u,
  LAN_CAN_BOOSTER_SET_TRACKPOWER = 0xCBu,
  LAN_FAST_CLOCK_CONTROL = 0xCCu,
  LAN_FAST_CLOCK_SETTINGS_GET = 0xCEu,
  LAN_FAST_CLOCK_SETTINGS_SET = 0xCFu,
  LAN_BOOSTER_SET_POWER = 0xB2u,
  LAN_BOOSTER_GET_DESCRIPTION = 0xB8u,
  LAN_BOOSTER_SET_DESCRIPTION = 0xB9u,
  LAN_BOOSTER_SYSTEMSTATE_GETDATA = 0xBBu,
  LAN_DECODER_GET_DESCRIPTION = 0xD8u,
  LAN_DECODER_SET_DESCRIPTION = 0xD9u,
  LAN_DECODER_SYSTEMSTATE_GETDATA = 0xDBu,
  LAN_ZLINK_GET_HWINFO = 0xE8u,

  // Z21 to Client
  Reply_to_LAN_GET_SERIAL_NUMBER = 0x10u,
  Reply_to_LAN_GET_COMMON_SETTINGS = 0x12u,
  Reply_to_LAN_GET_MMDCC_SETTINGS = 0x16u,
  Reply_to_LAN_GET_CODE = 0x18u,
  Reply_to_LAN_GET_HWINFO = 0x1Au,
  LAN_X_TURNOUT_INFO = 0x40u, LAN_X_EXT_ACCESSORY_INFO = 0x40u, LAN_X_BC_TRACK_POWER_OFF = 0x40u, LAN_X_BC_TRACK_POWER_ON = 0x40u, LAN_X_BC_PROGRAMMING_MODE = 0x40u, LAN_X_BC_TRACK_SHORT_CIRCUIT = 0x40u, LAN_X_CV_NACK_SC = 0x40u, LAN_X_CV_NACK = 0x40u, LAN_X_UNKNOWN_COMMAND = 0x40u, LAN_X_STATUS_CHANGED = 0x40u, Reply_to_LAN_X_GET_VERSION = 0x40u, LAN_X_CV_RESULT = 0x40u, LAN_X_BC_STOPPED = 0x40u, LAN_X_LOCO_INFO = 0x40u, Reply_to_LAN_X_GET_FIRMWARE_VERSION = 0x40u,
  Reply_to_LAN_GET_BROADCASTFLAGS = 0x51u,
  Reply_to_LAN_GET_LOCOMODE = 0x60u,
  Reply_to_LAN_GET_TURNOUTMODE = 0x70u,
  LAN_RMBUS_DATACHANGED = 0x80u,
  LAN_SYSTEMSTATE_DATACHANGED = 0x84u,
  LAN_RAILCOM_DATACHANGED = 0x88u,
  LAN_LOCONET_Z21_RX = 0xA0u,
  LAN_LOCONET_Z21_TX = 0xA1u,
  // LAN_LOCONET_FROM_LAN = 0xA2u,
  // LAN_LOCONET_DISPATCH_ADDR = 0xA3u,
  // LAN_LOCONET_DETECTOR = 0xA4u,
  // LAN_CAN_DETECTOR = 0xC4u,
  Reply_to_LAN_CAN_DEVICE_GET_DESCRIPTION = 0xC8u,
  LAN_CAN_BOOSTER_SYSTEMSTATE_CHANGED = 0xCAu,
  LAN_FAST_CLOCK_DATA = 0xCDu,
  // LAN_FAST_CLOCK_SETTINGS_GET = 0xCEu,
  Reply_to_LAN_BOOSTER_GET_DESCRIPTION = 0xB8u,
  LAN_BOOSTER_SYSTEMSTATE_DATACHANGED = 0xBAu,
  Reply_to_LAN_DECODER_GET_DESCRIPTION = 0xD8u,
  LAN_DECODER_SYSTEMSTATE_DATACHANGED = 0xDAu,
  Reply_to_LAN_ZLINK_GET_HWINFO = 0xE8u,
  // clang-format on
};

} // namespace z21