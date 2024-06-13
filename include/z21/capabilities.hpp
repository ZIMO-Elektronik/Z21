// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

///
///
/// \file   z21/capabilities.hpp
/// \author Vincent Hamp
/// \date   05/04/2024

#pragma once

#include <cstdint>
#include <ztl/enum.hpp>

namespace z21 {

///
enum class Capabilities : uint8_t {
  DCC = 0x01u,              ///< Capable of DCC
  MM = 0x02u,               ///< Capable of MM
  Reserved = 0x04u,         ///< Reserved for future development
  RailCom = 0x08u,          ///< RailCom is activated
  LocoCmds = 0x10u,         ///< Accepts LAN commands for locomotive decoders
  AccessoryCmds = 0x20u,    ///< Accepts LAN commands for accessory decoders
  DetectorCmds = 0x40u,     ///< Accepts LAN commands for detectors
  NeedsUnlockCode = 0x80u,  ///< Device needs activate code (z21start)
  All = DCC | MM | Reserved | RailCom | LocoCmds | AccessoryCmds | DetectorCmds,
};

ZTL_MAKE_ENUM_CLASS_FLAGS(Capabilities)

}  // namespace z21