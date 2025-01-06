// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// LocoNet interface
///
/// \file   z21/server/intf/loconet.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

namespace z21::server::intf {

/// TODO
struct LocoNet {
  LocoNet() { assert(false); }

  /// Dtor
  virtual ~LocoNet() = default;
};

} // namespace z21::server::intf