// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// DCC interface (combination of Driving, Switching, Programming and RailCom)
///
/// \file   z21/server/intf/dcc.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

#include "driving.hpp"
#include "programming.hpp"
#include "railcom.hpp"
#include "switching.hpp"

namespace z21::server::intf {

/// \todo document
struct Dcc : Driving, Switching, Programming, RailCom {};

} // namespace z21::server::intf
