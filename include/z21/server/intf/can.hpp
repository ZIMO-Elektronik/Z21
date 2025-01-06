// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// CAN interface
///
/// \file   z21/server/intf/can.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

namespace z21::server::intf {

/// TODO
struct Can {
  Can() { assert(false); }

  /// Dtor
  virtual ~Can() = default;
};

} // namespace z21::server::intf