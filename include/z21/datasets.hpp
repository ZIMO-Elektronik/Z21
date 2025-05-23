// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Datasets
///
/// \file   z21/datasets.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

#include <ztl/inplace_deque.hpp>
#include "dataset.hpp"

namespace z21 {

/// \todo document
using Datasets = ztl::inplace_deque<Dataset, Z21_SERVER_DEQUE_SIZE>;

} // namespace z21
