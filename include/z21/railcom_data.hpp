// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// RailCom data
///
/// \file   z21/railcom_data.hpp
/// \author Vincent Hamp
/// \date   17/04/2025

#pragma once

#include <cstdint>

namespace z21 {

/// \todo document
struct RailComData {
  friend constexpr bool operator==(RailComData const&,
                                   RailComData const&) = default;

  ///
  uint16_t loco_address{};

  ///
  uint32_t receive_counter{};

  ///
  uint16_t error_counter{};

  ///
  enum Options : uint8_t {
    Speed1 = 0x01u, // Subindex 0
    Speed2 = 0x02u, // Subindex 1
    QoS = 0x04u,    // Subindex 7
  } options{};

  ///
  uint8_t speed{};

  ///
  uint8_t qos{};
};

} // namespace z21
