// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

///
///
/// \file   z21/utility.hpp
/// \author Vincent Hamp
/// \date   05/04/2024

#pragma once

#include <cstdint>

namespace z21 {

/// Data to uint16_t
///
/// \param  data  Pointer to data
/// \return uint16_t from data
constexpr auto data2uint16(uint8_t const* data) {
  return static_cast<uint16_t>(data[0uz] << 8u | data[1uz] << 0u);
}

/// Data to uint32_t
///
/// \param  data  Pointer to data
/// \return uint32_t from data
constexpr auto data2uint32(uint8_t const* data) {
  return static_cast<uint32_t>(data[0uz] << 24u | data[1uz] << 16u |
                               data[2uz] << 8u | data[3uz] << 0u);
}

/// Data to loco address
///
/// \param  data  Pointer to data
/// \return loco address
constexpr auto data2loco_address(uint8_t const* data) {
  return static_cast<uint16_t>(data2uint16(data) & 0x3FFFu);
}

/// Data to CV address
///
/// \param  data  Pointer to data
/// \return CV address
constexpr auto data2cv_address(uint8_t const* data) {
  return static_cast<uint16_t>(data2uint16(data) & 0x03FFu);
}

/// uint16_t to data
///
/// \param  hword Half-word to convert
/// \param  data  Pointer to write to
/// \return Pointer after last element
constexpr auto uint16_2data(uint16_t hword, uint8_t* data) {
  *data++ = static_cast<uint8_t>((hword & 0xFF00u) >> 8u);
  *data++ = static_cast<uint8_t>((hword & 0x00FFu) >> 0u);
  return data;
}

/// uint32_t to data
///
/// \param  word  Word to convert
/// \param  data  Pointer to write to
/// \return Pointer after last element
constexpr auto uint32_2data(uint32_t word, uint8_t* data) {
  *data++ = static_cast<uint8_t>((word & 0xFF00'0000u) >> 24u);
  *data++ = static_cast<uint8_t>((word & 0x00FF'0000u) >> 16u);
  *data++ = static_cast<uint8_t>((word & 0x0000'FF00u) >> 8u);
  *data++ = static_cast<uint8_t>((word & 0x0000'00FFu) >> 0u);
  return data;
}

}  // namespace z21