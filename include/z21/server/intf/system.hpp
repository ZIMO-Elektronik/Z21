// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// System interface
///
/// \file   z21/server/intf/system.hpp
/// \author Vincent Hamp
/// \date   17/04/2024

#pragma once

#include <cstdint>
#include <span>
#include "../../socket.hpp"

namespace z21::server::intf {

/// TODO
///
/// Commands:
/// - LAN_GET_SERIAL
/// - LAN_GET_SERIAL_NUMBER
/// - LAN_GET_CODE
/// - LAN_GET_HWINFO
/// - LAN_LOGOFF
/// - LAN_X_GET_VERSION
/// - LAN_X_GET_STATUS
/// - LAN_X_SET_TRACK_POWER_OFF
/// - LAN_X_SET_TRACK_POWER_ON
/// - LAN_X_GET_FIRMWARE_VERSION
/// - LAN_SET_BROADCASTFLAGS
/// - LAN_GET_BROADCASTFLAGS
/// - LAN_SYSTEMSTATE_GETDATA
///
/// Replies:
/// - Reply_to_LAN_GET_SERIAL_NUMBER
/// - Reply_to_LAN_GET_CODE = 0x18u,
/// - Reply_to_LAN_GET_HWINFO = 0x1Au,
/// - LAN_X_BC_TRACK_POWER_OFF
/// - LAN_X_BC_TRACK_POWER_ON
/// - LAN_X_BC_PROGRAMMING_MODE
/// - LAN_X_BC_TRACK_SHORT_CIRCUIT
/// - LAN_X_UNKNOWN_COMMAND
/// - LAN_X_STATUS_CHANGED
/// - Reply_to_LAN_X_GET_VERSION
/// - LAN_X_BC_STOPPED
/// - Reply_to_LAN_X_GET_FIRMWARE_VERSION
/// - Reply_to_LAN_GET_BROADCASTFLAGS
/// - LAN_SYSTEMSTATE_DATACHANGED
struct System {
  /// Dtor
  virtual ~System() = default;

  //
  virtual void trackPower(bool on) = 0;
  virtual void stop() = 0;

  //
  virtual void broadcastTrackPowerOff() = 0;
  virtual void broadcastTrackPowerOn() = 0;
  virtual void broadcastProgrammingMode() = 0;
  virtual void broadcastTrackShortCircuit() = 0;
  virtual void broadcastStopped() = 0;
  virtual void broadcastSystemStateData() = 0;

  //
  virtual int32_t serialNumber() const { return 0; }
  virtual void logoff(Socket const&) {}

  //
  virtual int16_t mainCurrent() const { return 0; }
  virtual int16_t progCurrent() const { return 0; }
  virtual int16_t filteredMainCurrent() const { return 0; }
  virtual int16_t temperature() const { return 0; }
  virtual uint16_t supplyVoltage() const { return 0u; }
  virtual uint16_t vccVoltage() const { return 0u; }
};

}  // namespace z21::server::intf