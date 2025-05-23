// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Broadcast flags
///
/// \file   z21/broadcast_flags.hpp
/// \author Vincent Hamp
/// \date   05/04/2024

#pragma once

#include <cstdint>
#include <cstdio>
#include <ztl/enum.hpp>

namespace z21 {

/// Broadcast flags assigned per client
///
/// These flags determine whether a client has subscribed to a broadcast message
/// or not. Only subscribed clients receive the respective messages.
enum class BroadcastFlags : uint32_t {
  DrivingSwitching = 0x00000001u,
  RBus = 0x00000002u,
  RailComSubscribed = 0x00000004u,
  SystemStatus = 0x00000100u,
  DrivingSwitchingEx = 0x00010000u,
  LocoNet = 0x01000000u,
  LocoNetDriving = 0x02000000u,
  LocoNetSwitching = 0x04000000u,
  LocoNetDetector = 0x08000000u,
  RailCom = 0x00040000u,
  CANDetector = 0x00080000u,
  CANBooster = 0x00020000u,
  FastClock = 0x00000010u,
  All = DrivingSwitching | RBus | RailComSubscribed | SystemStatus |
        DrivingSwitchingEx | LocoNet | LocoNetDriving | LocoNetSwitching |
        LocoNetDetector | RailCom | CANDetector | CANBooster | FastClock,
  Default = DrivingSwitching | RBus | LocoNet | LocoNetDetector,
};

ZTL_MAKE_ENUM_CLASS_FLAGS(BroadcastFlags)

/// \todo remove
inline void print_broadcast_flags(BroadcastFlags flags,
                                  decltype(printf) fp = printf) {
  fp("broadcast flags: ");
  if (std::to_underlying(flags & BroadcastFlags::DrivingSwitching))
    fp("%s ", "DrivingSwitching");
  if (std::to_underlying(flags & BroadcastFlags::RBus)) fp("%s ", "RBus");
  if (std::to_underlying(flags & BroadcastFlags::RailComSubscribed))
    fp("%s ", "RailComSubscribed");
  if (std::to_underlying(flags & BroadcastFlags::SystemStatus))
    fp("%s ", "SystemStatus");
  if (std::to_underlying(flags & BroadcastFlags::DrivingSwitchingEx))
    fp("%s ", "DrivingSwitchingEx");
  if (std::to_underlying(flags & BroadcastFlags::LocoNet)) fp("%s ", "LocoNet");
  if (std::to_underlying(flags & BroadcastFlags::LocoNetDriving))
    fp("%s ", "LocoNetDriving");
  if (std::to_underlying(flags & BroadcastFlags::LocoNetSwitching))
    fp("%s ", "LocoNetSwitching");
  if (std::to_underlying(flags & BroadcastFlags::LocoNetDetector))
    fp("%s ", "LocoNetDetector");
  if (std::to_underlying(flags & BroadcastFlags::RailCom)) fp("%s ", "RailCom");
  if (std::to_underlying(flags & BroadcastFlags::CANDetector))
    fp("%s ", "CANDetector");
  if (std::to_underlying(flags & BroadcastFlags::CANBooster))
    fp("%s ", "CANBooster");
  if (std::to_underlying(flags & BroadcastFlags::FastClock))
    fp("%s ", "FastClock");
  fp("\n");
}

} // namespace z21
