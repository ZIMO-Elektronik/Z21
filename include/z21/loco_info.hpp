// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Loco info
///
/// \file   z21/loco_info.hpp
/// \author Vincent Hamp
/// \date   10/04/2024

#pragma once

#include <cstdint>
#include <ztl/bits.hpp>

namespace z21 {

/// \todo document
struct LocoInfo {
  friend constexpr bool operator==(LocoInfo const&, LocoInfo const&) = default;

  enum Mode : uint8_t {
    DCC = 0u,
    MM = 1u,
  } mode : 1 {DCC};

  bool busy : 1 {};

  enum SpeedSteps : uint8_t {
    DCC14 = 0u,
    DCC28 = 2u,
    DCC128 = 4u,
  } speed_steps : 3 {DCC128};

  uint8_t rvvvvvvv{ztl::mask<7u>};

  bool double_traction : 1 {};
  bool smart_search : 1 {};

  uint32_t f31_0{};
};

/// Decode direction and speed byte
///
/// \param  speed_steps Speed steps
/// \param  rvvvvvvv    Direction and speed byte
/// \retval -1          EStop
/// \retval 0           Stop
/// \retval >0          Speed
constexpr int32_t decode_rvvvvvvv(LocoInfo::SpeedSteps speed_steps,
                                  uint8_t rvvvvvvv) {
  // 128 speed steps
  if (speed_steps == LocoInfo::DCC128) {
    // Halt
    if (!(rvvvvvvv & 0b0111'1111u)) return 0;
    // EStop
    else if (!(rvvvvvvv & 0b0111'1110u)) return -1;
    else return static_cast<int32_t>(rvvvvvvv & 0b0111'1111u) - 1;
  }
  // 14 or 28 speed steps
  else {
    // Halt
    if (!(rvvvvvvv & 0b0000'1111u)) return 0;
    // EStop
    else if (!(rvvvvvvv & 0b0000'1110u)) return -1;

    int32_t speed{static_cast<int32_t>(rvvvvvvv & 0b0000'1111u) - 1};

    // 28 speed steps with intermediate
    if (speed_steps == LocoInfo::DCC28) {
      speed <<= 1;
      if (!(rvvvvvvv & ztl::mask<4u>)) --speed;
    }

    return speed;
  }
}

/// Encode direction and speed byte
///
/// \param  speed_steps Speed steps
/// \param  dir         Direction
/// \param  speed       Speed
/// \return Direction and speed byte
constexpr uint8_t
encode_rvvvvvvv(LocoInfo::SpeedSteps speed_steps, bool dir, int32_t speed) {
  // Halt
  if (!speed) return static_cast<uint8_t>(dir << 7u);
  // EStop
  else if (speed < 0) return static_cast<uint8_t>(dir << 7u) | 0b1u;

  auto vvvvvvv{static_cast<uint32_t>(speed + 1)};

  // 28 speed steps with intermediate
  if (speed_steps == LocoInfo::DCC28) {
    vvvvvvv = (vvvvvvv >> 1) + 1;
    if (!(speed % 2)) vvvvvvv |= ztl::mask<4u>;
  }

  return static_cast<uint8_t>(static_cast<uint32_t>(dir << 7u) | vvvvvvv);
}

} // namespace z21
