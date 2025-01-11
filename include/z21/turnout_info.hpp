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

///
struct TurnoutInfo {
  friend constexpr bool operator==(TurnoutInfo const&,
                                   TurnoutInfo const&) = default;

  enum Mode : uint8_t {
    DCC = 0u,
    MM = 1u,
  } mode : 1 {DCC};

  enum State : uint8_t {
    Unknown,
    P0,
    P1,
    Invalid,
  } state : 2 {};
};

} // namespace z21