// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// System state
///
/// \file   z21/system_state.hpp
/// \author Vincent Hamp
/// \date   10/04/2024

#pragma once

#include "capabilities.hpp"
#include "central_state.hpp"
#include "central_state_ex.hpp"

namespace z21 {

///
struct SystemState {
  friend constexpr bool operator==(SystemState const&,
                                   SystemState const&) = default;

  int16_t main_current{};
  int16_t prog_current{};
  int16_t filtered_main_current{};
  int16_t temperature{};
  uint16_t supply_voltage{};
  uint16_t vcc_voltage{};
  CentralState central_state{CentralState::TrackVoltageOff};
  CentralStateEx central_state_ex{};
  Capabilities capabilities{Capabilities::All};
};

} // namespace z21