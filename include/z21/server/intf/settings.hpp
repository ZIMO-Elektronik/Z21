// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Settings interface
///
/// \file   z21/server/intf/settings.hpp
/// \author Vincent Hamp
/// \date   12/06/2024

#pragma once

namespace z21::server::intf {

/// TODO
struct Settings {
  Settings() { assert(false); }

  /// Dtor
  virtual ~Settings() = default;
};

}  // namespace z21::server::intf