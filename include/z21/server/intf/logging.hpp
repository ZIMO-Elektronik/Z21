// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Logging interface
///
/// \file   z21/server/intf/logging.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

namespace z21::server::intf {

/// TODO
struct Logging {
  /// Dtor
  virtual ~Logging() = default;

  virtual void log(char const* str) = 0;
};

} // namespace z21::server::intf