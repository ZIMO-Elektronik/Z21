// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Fast clock interface
///
/// \file   z21/server/intf/fast_clock.hpp
/// \author Vincent Hamp
/// \date   12/06/2024

#pragma once

#include <cassert>

namespace z21::server::intf {

/// \todo document
struct FastClock {
  FastClock() { assert(false); }

  /// Dtor
  virtual ~FastClock() = default;
};

} // namespace z21::server::intf
