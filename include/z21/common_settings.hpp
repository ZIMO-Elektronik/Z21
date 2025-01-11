// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Common settings
///
/// \file   z21/common_settings.hpp
/// \author Vincent Hamp
/// \date   04/01/2025

#pragma once

#include <cstdint>
#include <ztl/enum.hpp>

namespace z21 {

///
struct CommonSettings {
  friend constexpr bool operator==(CommonSettings const&,
                                   CommonSettings const&) = default;

  bool enable_railcom{};
  bool enable_bit_modify_on_long_address{};
  uint8_t key_stop_mode{};
  uint8_t programming_type{};
  bool enable_loconet_current_source{};
  uint8_t loconet_fast_clock_rate{};
  uint8_t loconet_mode{};
  uint8_t ext_settings{};
  uint8_t purging_time{};
  uint8_t bus_settings{};
};

} // namespace z21