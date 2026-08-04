// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Loco entry
///
/// \file   z21/loco_entry.hpp
/// \author Vincent Hamp
/// \date   29/07/2026

#pragma once

#include <cstdint>
#include <string>

namespace z21 {

/// \todo document
struct LocoEntry {
  friend constexpr bool operator==(LocoEntry const&,
                                   LocoEntry const&) = default;

  uint8_t index{};
  uint8_t size{};
  std::string name{};
};

} // namespace z21
