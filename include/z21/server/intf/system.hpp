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
#include "../../system_state.hpp"

namespace z21::server::intf {

/// \todo
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
  /// Ctor
  explicit constexpr System(SystemState initial_sys_state)
    : _sys_state{initial_sys_state} {}

  /// Dtor
  virtual ~System() = default;

  // Required

  /// \todo
  [[nodiscard]] virtual bool trackPower(bool on) = 0;

  /// \todo
  [[nodiscard]] virtual bool stop() = 0;

  // Optional

  /// \todo
  [[nodiscard]] virtual int32_t serialNumber() const { return 0; }

  /// \todo
  virtual void logoff(Socket const&) {}

  /// \todo
  [[nodiscard]] virtual SystemState& systemState() { return _sys_state; }

  // Implemented by Base

  /// \todo
  virtual void broadcastTrackPowerOff() = 0;

  /// \todo
  virtual void broadcastTrackPowerOn() = 0;

  /// \todo
  virtual void broadcastProgrammingMode() = 0;

  /// \todo
  virtual void broadcastTrackShortCircuit() = 0;

  /// \todo
  virtual void broadcastStopped() = 0;

  /// \todo
  virtual void broadcastSystemStateData() = 0;

private:
  SystemState _sys_state{};
};

} // namespace z21::server::intf