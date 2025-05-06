// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Interface concept
///
/// \file   z21/server/intf/interface.hpp
/// \author Vincent Hamp
/// \date   17/04/2024

#pragma once

#include <ztl/type_traits.hpp>
#include "can.hpp"
#include "dcc.hpp"
#include "driving.hpp"
#include "fast_clock.hpp"
#include "loconet.hpp"
#include "logging.hpp"
#include "programming.hpp"
#include "railcom.hpp"
#include "rbus.hpp"
#include "settings.hpp"
#include "switching.hpp"
#include "system.hpp"
#include "zlink.hpp"

namespace z21::server::intf {

/// \todo document
template<typename T>
concept Interface = ztl::contains_type_v<T,
                                         std::tuple<intf::Can,
                                                    intf::Dcc,
                                                    intf::Driving,
                                                    intf::FastClock,
                                                    intf::LocoNet,
                                                    intf::Logging,
                                                    intf::Programming,
                                                    intf::RailCom,
                                                    intf::RBus,
                                                    intf::Settings,
                                                    intf::Switching,
                                                    intf::System, // Required
                                                    intf::ZLink>>;

} // namespace z21::server::intf