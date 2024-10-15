// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Extended central state
///
/// \file   z21/central_state_ex.hpp
/// \author Vincent Hamp
/// \date   05/04/2024

#pragma once

#include <cstdint>
#include <ztl/enum.hpp>

namespace z21 {

///
enum class CentralStateEx : uint8_t {
  HighTemperature = 0x01,        ///< Temperature too high
  PowerLost = 0x02u,             ///< Input voltage too low
  ShortCircuitExternal = 0x04u,  ///< S.C. at the external booster output
  ShortCircuitInternal =
    0x08u,             ///< S.C. at the main track or programming track
  RevPol = 0x10u,      ///< Supply voltage error
  RCN213 = 0x20u,      ///< Turnout addresses according to RCN-213
  NoDCCInput = 0x80u,  ///< No DCC input signal
  All = HighTemperature | PowerLost | ShortCircuitExternal |
        ShortCircuitInternal | RevPol | RCN213 | NoDCCInput,
};

ZTL_MAKE_ENUM_CLASS_FLAGS(CentralStateEx)

}  // namespace z21