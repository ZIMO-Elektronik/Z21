// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

///
///
/// \file   z21/central_state.hpp
/// \author Vincent Hamp
/// \date   05/04/2024

#pragma once

#include <cstdint>
#include <ztl/enum.hpp>

namespace z21 {

///
enum class CentralState : uint8_t {
  EmergencyStop = 0x01u,          ///< The emergency stop for decoder
  TrackVoltageOff = 0x02u,        ///< The track voltage is switched off
  ShortCircuit = 0x04u,           ///< Short-circuit
  ConfigMode = 0x10u,             ///< Configuration mode is active
  ProgrammingModeActive = 0x20u,  ///< The programming mode is active
  All = EmergencyStop | TrackVoltageOff | ShortCircuit | ConfigMode |
        ProgrammingModeActive,
};

ZTL_MAKE_ENUM_CLASS_FLAGS(CentralState)

}  // namespace z21