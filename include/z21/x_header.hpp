// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// X-Header
///
/// \file   z21/x_header.hpp
/// \author Vincent Hamp
/// \date   10/11/2023

#pragma once

#include <cstdint>

namespace z21 {

/// \todo document
enum class XHeader : uint8_t {
  // clang-format off
  // Client to Z21
  LAN_X_21 = 0x21u, LAN_X_GET_VERSION = 0x21u, LAN_X_GET_STATUS = 0x21u, LAN_X_SET_TRACK_POWER_OFF = 0x21u, LAN_X_SET_TRACK_POWER_ON = 0x21u,
  LAN_X_DCC_READ_REGISTER = 0x22u,
  LAN_X_23 = 0x23u, LAN_X_CV_READ = 0x23u, LAN_X_DCC_WRITE_REGISTER = 0x23u,
  LAN_X_24 = 0x24u, LAN_X_CV_WRITE = 0x24u, LAN_X_MM_WRITE_BYTE = 0x24u,
  LAN_X_GET_TURNOUT_INFO = 0x43u,
  LAN_X_GET_EXT_ACCESSORY_INFO = 0x44u,
  LAN_X_SET_TURNOUT = 0x53u,
  LAN_X_SET_EXT_ACCESSORY = 0x54u,
  LAN_X_SET_STOP = 0x80u,
  LAN_X_SET_LOCO_E_STOP = 0x92u,
  LAN_X_E3 = 0xE3u, LAN_X_PURGE_LOCO = 0xE3u, LAN_X_GET_LOCO_INFO = 0xE3u,
  LAN_X_E4 = 0xE4u, LAN_X_SET_LOCO_DRIVE = 0xE4u, LAN_X_SET_LOCO_FUNCTION = 0xE4u, LAN_X_SET_LOCO_FUNCTION_GROUP = 0xE4u, LAN_X_SET_LOCO_BINARY_STATE = 0xE4u,
  LAN_X_E6 = 0xE6u, LAN_X_CV_POM_WRITE_BYTE = 0xE6u, LAN_X_CV_POM_WRITE_BIT = 0xE6u, LAN_X_CV_POM_READ_BYTE = 0xE6u, LAN_X_CV_POM_ACCESSORY_WRITE_BYTE = 0xE6u, LAN_X_CV_POM_ACCESSORY_WRITE_BIT = 0xE6u, LAN_X_CV_POM_ACCESSORY_READ_BYTE = 0xE6u,
  LAN_X_GET_FIRMWARE_VERSION = 0xF1u,

  // Z21 to Client
  LAN_X_TURNOUT_INFO = 0x43u,
  LAN_X_EXT_ACCESSORY_INFO = 0x44u,
  LAN_X_61 = 0x61u, LAN_X_BC_TRACK_POWER_OFF = 0x61u, LAN_X_BC_TRACK_POWER_ON = 0x61u, LAN_X_BC_PROGRAMMING_MODE = 0x61u, LAN_X_BC_TRACK_SHORT_CIRCUIT = 0x61u, LAN_X_CV_NACK_SC = 0x61u, LAN_X_CV_NACK = 0x61u, LAN_X_UNKNOWN_COMMAND = 0x61u,
  LAN_X_STATUS_CHANGED = 0x62u,
  Reply_to_LAN_X_GET_VERSION = 0x63u,
  LAN_X_CV_RESULT = 0x64u,
  LAN_X_BC_STOPPED = 0x81u,
  LAN_X_LOCO_INFO = 0xEFu,
  Reply_to_LAN_X_GET_FIRMWARE_VERSION = 0xF3u,
  // clang-format on
};

} // namespace z21