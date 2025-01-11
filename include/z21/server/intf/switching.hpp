// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Switching interface
///
/// \file   z21/server/intf/switching.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

#include "../../accessory_info.hpp"
#include "../../turnout_info.hpp"

namespace z21::server::intf {

/// \todo
///
/// Commands:
/// - LAN_X_GET_TURNOUT_INFO
/// - LAN_X_GET_EXT_ACCESSORY_INFO
/// - LAN_X_SET_TURNOUT
/// - LAN_X_SET_EXT_ACCESSORY
/// - LAN_X_GET_TURNOUTMODE
/// - LAN_X_SET_TURNOUTMODE
///
/// Replies:
/// - LAN_X_TURNOUT_INFO
/// - LAN_X_EXT_ACCESSORY_INFO
/// - Reply_to_LAN_GET_TURNOUTMODE
struct Switching {
  /// Dtor
  virtual ~Switching() = default;

  // Required

  /// \todo
  [[nodiscard]] virtual TurnoutInfo turnoutInfo(uint16_t accy_addr) = 0;

  /// \todo
  [[nodiscard]] virtual AccessoryInfo accessoryInfo(uint16_t accy_addr) = 0;

  /// \todo
  virtual void turnout(uint16_t accy_addr, bool p, bool a, bool q) = 0;

  /// \todo
  virtual void accessory(uint16_t accy_addr, uint8_t dddddddd) = 0;

  /// \todo
  [[nodiscard]] virtual TurnoutInfo::Mode turnoutMode(uint16_t accy_addr) = 0;

  /// \todo
  virtual void turnoutMode(uint16_t accy_addr, TurnoutInfo::Mode mode) = 0;

  // Implemented by Base

  /// \todo
  virtual void broadcastTurnoutInfo(uint16_t accy_addr) = 0;

  /// \todo
  virtual void broadcastExtAccessoryInfo(uint16_t accy_addr) = 0;
};

} // namespace z21::server::intf