// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Dataset
///
/// \file   z21/dataset.hpp
/// \author Vincent Hamp
/// \date   09/11/2023

#pragma once

#include <cstdint>
#include <ztl/inplace_vector.hpp>
#include "header.hpp"

namespace z21 {

///
struct Dataset {
  Header header{};
  ztl::inplace_vector<uint8_t, Z21_MAX_DATASET_SIZE> chunk{};
};

} // namespace z21