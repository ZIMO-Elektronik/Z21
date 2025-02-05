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

///
struct AccessoryInfo : TurnoutInfo {
  friend constexpr bool operator==(AccessoryInfo const&,
                                   AccessoryInfo const&) = default;

  uint8_t dddddddd{};

  enum Status : uint8_t {
    Valid = 0x00u,
    Unknown = 0xFFu,
  } status{Unknown};
};

///
template<typename Accessory>
constexpr bool is_ext_accessory(Accessory&& accessory)
  requires(std::same_as<std::remove_cvref_t<Accessory>, TurnoutInfo> ||
           std::same_as<std::remove_cvref_t<Accessory>, AccessoryInfo>)
{
  if constexpr (std::same_as<std::remove_cvref_t<Accessory>, TurnoutInfo>)
    return false;
  else if constexpr (std::same_as<std::remove_cvref_t<Accessory>,
                                  AccessoryInfo>)
    return accessory.status == z21::AccessoryInfo::Status::Valid;
}

} // namespace z21