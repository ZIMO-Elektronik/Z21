// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Hardware type
///
/// \file   z21/hw_type.hpp
/// \author Vincent Hamp
/// \date   30/04/2024

#pragma once

#include <cstdint>

namespace z21 {

///
enum class HwType : uint32_t {
  Z21_OLD = 0x0000'0200u,    // „black Z21” (hardware variant from 2012)
  Z21_NEW = 0x0000'0201u,    // „black Z21”(hardware variant from 2013)
  SMARTRAIL = 0x0000'0202u,  // SmartRail (from 2012)
  z21_SMALL = 0x0000'0203u,  // „white z21” starter set variant (from 2013)
  z21_START = 0x0000'0204u,  // „z21 start” starter set variant (from 2016)

  SINGLE_BOOSTER = 0x0000'0205u,  // 10806 „Z21 Single Booster” (zLink)
  DUAL_BOOSTER = 0x0000'0206u,    // 10807 „Z21 Dual Booster” (zLink)

  Z21_XL = 0x0000'0211u,      // 10870 „Z21 XL Series” (from 2020)
  XL_BOOSTER = 0x0000'0212u,  // 10869 „Z21 XL Booster” (from 2021, zLink)

  Z21_SWITCH_DECODER = 0x0000'0301u,  // 10836 „Z21 SwitchDecoder” (zLink)
  Z21_SIGNAL_DECODER = 0x0000'0302u,  // 10836 „Z21 SignalDecoder” (zLink)
};

}  // namespace z21