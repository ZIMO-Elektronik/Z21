// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// RailCom interface
///
/// \file   z21/server/intf/railcom.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

#include "../../railcom_data.hpp"

namespace z21::server::intf {

/// \todo
///
/// Commands:
/// - LAN_RAILCOM_GETDATA
///
/// Replies:
/// - LAN_RAILCOM_DATACHANGED
struct RailCom {
  /// Dtor
  virtual ~RailCom() = default;

  /// \todo
  [[nodiscard]] virtual RailComData railComData(uint16_t loco_addr) = 0;

  // Implemented by Base

  /// \todo
  virtual void broadcastRailComData(uint16_t loco_addr) = 0;
};

} // namespace z21::server::intf