// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Accessory info
///
/// \file   z21/accessory_info.hpp
/// \author Vincent Hamp
/// \date   09/01/2025

#pragma once

#include <cstdint>
#include "turnout_info.hpp"

namespace z21 {

/// \todo document
struct AccessoryInfo {
  friend constexpr bool operator==(AccessoryInfo const&,
                                   AccessoryInfo const&) = default;

  uint8_t dddddddd{};

  enum Status : uint8_t {
    Valid = 0x00u,
    Unknown = 0xFFu,
  } status{Unknown};
};

} // namespace z21