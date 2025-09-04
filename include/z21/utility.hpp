// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Utility
///
/// \file   z21/utility.hpp
/// \author Vincent Hamp
/// \date   05/04/2024

#pragma once

#include <cstdint>

namespace z21 {

/// Big endian data to uint16_t
///
/// \param  data  Pointer to data
/// \return uint16_t from data
constexpr auto big_endian_data2uint16(uint8_t const* data) {
  return static_cast<uint16_t>(data[0uz] << 8u | data[1uz] << 0u);
}

/// Little endian data to uint16_t
///
/// \param  data  Pointer to data
/// \return uint16_t from data
constexpr auto little_endian_data2uint16(uint8_t const* data) {
  return static_cast<uint16_t>(data[1uz] << 8u | data[0uz] << 0u);
}

/// Little endian data to uint32_t
///
/// \param  data  Pointer to data
/// \return uint32_t from data
constexpr auto little_endian_data2uint32(uint8_t const* data) {
  return static_cast<uint32_t>(data[3uz] << 24u | data[2uz] << 16u |
                               data[1uz] << 8u | data[0uz] << 0u);
}

/// Big endian data to loco address
///
/// \param  data  Pointer to data
/// \return Loco address
constexpr auto big_endian_data2loco_address(uint8_t const* data) {
  return static_cast<uint16_t>(big_endian_data2uint16(data) & 0x3FFFu);
}

/// Big endian data to CV address
///
/// \param  data  Pointer to data
/// \return CV address
constexpr auto big_endian_data2cv_address(uint8_t const* data) {
  return static_cast<uint16_t>(big_endian_data2uint16(data) & 0x03FFu);
}

/// Big endian data to accessory address
///
/// \param  data  Pointer to data
/// \return Accessory address
constexpr auto big_endian_data2accessory_address(uint8_t const* data) {
  return static_cast<uint16_t>(big_endian_data2uint16(data) & 0x07FFu);
}

/// Stupid data to accessory address
///
/// \warning
/// This encoding is used by the LAN_X_CV_POM_ACCESSORY_* commands. I don't know
/// WHAT THE FUCK they where thinking.
///
/// \param  data  Pointer to data
/// \return Accessory address
constexpr auto stupid_data2accessory_address(uint8_t const* data) {
  return static_cast<uint16_t>(
    (data[0uz] << 7u | (data[1uz] & 0xF0u) >> 2u | (data[1uz] & 0x03u)) &
    0x07FFu);
}

/// Little endian data to loco address
///
/// \param  data  Pointer to data
/// \return Loco address
constexpr auto little_endian_data2loco_address(uint8_t const* data) {
  return static_cast<uint16_t>(little_endian_data2uint16(data) & 0x3FFFu);
}

} // namespace z21
