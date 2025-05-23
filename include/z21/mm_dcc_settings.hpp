// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// MM/DCC settings
///
/// \file   z21/mm_dcc_settings.hpp
/// \author Vincent Hamp
/// \date   04/01/2025

#pragma once

#include <cstdint>
#include <ztl/enum.hpp>

namespace z21 {

/// \todo document
struct MmDccSettings {
  friend constexpr bool operator==(MmDccSettings const&,
                                   MmDccSettings const&) = default;

  ///
  uint8_t startup_reset_package_count{};

  ///
  uint8_t continue_reset_packet_count{};

  ///
  uint8_t program_package_count{};

  ///
  bool bit_verify_to_one{};

  ///
  uint8_t programming_ack_current{};

  ///
  enum Flags : uint8_t {
    DccMmMixed = 0x00u,
    DccOnly = 0x02u,
    MmOnly = 0x03u,

    ///
    /// \warning
    /// This does not correspond to the official specification.
    CV29AutomaticAddress = 0x20u,

    ///
    RepeatHfx = 0x40u,

    ///
    DccShort127 = 0x80u,
  } flags{};

  ///
  uint16_t output_voltage{};

  ///
  uint16_t programming_voltage{};
};

} // namespace z21
