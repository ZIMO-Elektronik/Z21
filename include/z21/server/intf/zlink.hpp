// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// ZLink interface
///
/// \file   z21/server/intf/zlink.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

namespace z21::server::intf {

/// \todo document
struct ZLink {
  ZLink() { assert(false); }

  /// Dtor
  virtual ~ZLink() = default;
};

} // namespace z21::server::intf
