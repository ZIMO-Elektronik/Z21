// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Settings interface
///
/// \file   z21/server/intf/settings.hpp
/// \author Vincent Hamp
/// \date   12/06/2024

#pragma once

#include "../../common_settings.hpp"
#include "../../mm_dcc_settings.hpp"

namespace z21::server::intf {

/// \todo
///
/// Commands:
/// - LAN_GET_COMMON_SETTINGS
/// - LAN_GET_MMDCC_SETTINGS
///
/// Replies:
/// - LAN_SET_COMMON_SETTINGS
/// - LAN_SET_MMDCC_SETTINGS
struct Settings {
  /// Dtor
  virtual ~Settings() = default;

  [[nodiscard]] virtual CommonSettings commonSettings() = 0;
  virtual void commonSettings(CommonSettings const&) = 0;

  [[nodiscard]] virtual MmDccSettings mmDccSettings() = 0;
  virtual void mmDccSettings(MmDccSettings const&) = 0;
};

} // namespace z21::server::intf