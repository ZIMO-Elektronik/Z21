// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Turnout info
///
/// \file   z21/turnout_info.hpp
/// \author Vincent Hamp
/// \date   08/01/2025

#pragma once

#include <cstdint>

namespace z21 {

/// \todo document
struct TurnoutInfo {
  friend constexpr bool operator==(TurnoutInfo const&,
                                   TurnoutInfo const&) = default;

  enum Mode : uint8_t {
    DCC = 0u,
    MM = 1u,
  } mode : 1 {DCC};

  enum Position : uint8_t {
    Unknown = 0b00u,
    P0 = 0b01u,
    P1 = 0b10u,
    Invalid = 0b11u,
  } position : 2 {Unknown};
};

} // namespace z21