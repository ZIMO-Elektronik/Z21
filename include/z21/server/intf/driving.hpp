// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Driving interface
///
/// \file   z21/server/intf/driving.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

#include "../../loco_info.hpp"

namespace z21::server::intf {

/// \todo document
///
/// Commands:
/// - LAN_X_SET_LOCO_E_STOP
/// - LAN_X_PURGE_LOCO
/// - LAN_X_GET_LOCO_INFO
/// - LAN_X_SET_LOCO_DRIVE
/// - LAN_X_SET_LOCO_FUNCTION
/// - LAN_X_SET_LOCO_FUNCTION_GROUP
/// - LAN_X_SET_LOCO_BINARY_STATE
/// - LAN_GET_LOCOMODE
/// - LAN_SET_LOCOMODE
///
/// Replies:
/// - LAN_X_LOCO_INFO
/// - Reply_to_LAN_GET_LOCOMODE
struct Driving {
  /// Dtor
  virtual ~Driving() = default;

  // Required

  /// \todo
  [[nodiscard]] virtual LocoInfo locoInfo(uint16_t loco_addr) = 0;

  /// \todo
  virtual void locoDrive(uint16_t loco_addr,
                         LocoInfo::SpeedSteps speed_steps,
                         uint8_t rvvvvvvv) = 0;

  /// \todo
  virtual void
  locoFunction(uint16_t loco_addr, uint32_t mask, uint32_t state) = 0;

  /// \todo
  [[nodiscard]] virtual LocoInfo::Mode locoMode(uint16_t loco_addr) = 0;

  /// \todo
  virtual void locoMode(uint16_t loco_addr, LocoInfo::Mode mode) = 0;

  // Implemented by Base

  /// \todo
  virtual void broadcastLocoInfo(uint16_t loco_addr) = 0;
};

} // namespace z21::server::intf
