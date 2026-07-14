// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Utilities
///
/// \file   z21/utility.hpp
/// \author Vincent Hamp
/// \date   05/04/2024

#pragma once

#include <cstdint>
#include <iterator>

namespace z21 {

/// Big endian data to uint16_t
///
/// \tparam RandomIt  std::random_access_iterator
/// \param  first     Beginning of the source range
/// \return uint16_t
template<std::random_access_iterator RandomIt>
requires(sizeof(std::iter_value_t<RandomIt>) == 1uz)
constexpr uint16_t big_endian_data2uint16(RandomIt first) {
  return static_cast<uint16_t>(static_cast<uint32_t>(first[0uz]) << 8u |
                               static_cast<uint32_t>(first[1uz]) << 0u);
}

/// Little endian data to uint16_t
///
/// \tparam RandomIt  std::random_access_iterator
/// \param  first     Beginning of the source range
/// \return uint16_t
template<std::random_access_iterator RandomIt>
requires(sizeof(std::iter_value_t<RandomIt>) == 1uz)
constexpr uint16_t little_endian_data2uint16(RandomIt first) {
  return static_cast<uint16_t>(static_cast<uint32_t>(first[1uz]) << 8u |
                               static_cast<uint32_t>(first[0uz]) << 0u);
}

/// Little endian data to uint32_t
///
/// \tparam RandomIt  std::random_access_iterator
/// \param  first     Beginning of the source range
/// \return uint32_t
template<std::random_access_iterator RandomIt>
requires(sizeof(std::iter_value_t<RandomIt>) == 1uz)
constexpr uint32_t little_endian_data2uint32(RandomIt first) {
  return static_cast<uint32_t>(first[3uz]) << 24u |
         static_cast<uint32_t>(first[2uz]) << 16u |
         static_cast<uint32_t>(first[1uz]) << 8u |
         static_cast<uint32_t>(first[0uz]) << 0u;
}

/// Big endian data to loco address
///
/// \tparam RandomIt  std::random_access_iterator
/// \param  first     Beginning of the source range
/// \return Loco address
template<std::random_access_iterator RandomIt>
requires(sizeof(std::iter_value_t<RandomIt>) == 1uz)
constexpr uint16_t big_endian_data2loco_address(RandomIt first) {
  return static_cast<uint16_t>(big_endian_data2uint16(first) & 0x3FFFu);
}

/// Big endian data to CV address
///
/// \tparam RandomIt  std::random_access_iterator
/// \param  first     Beginning of the source range
/// \return CV address
template<std::random_access_iterator RandomIt>
requires(sizeof(std::iter_value_t<RandomIt>) == 1uz)
constexpr uint16_t big_endian_data2cv_address(RandomIt first) {
  return static_cast<uint16_t>(big_endian_data2uint16(first) & 0x03FFu);
}

/// Big endian data to accessory address
///
/// \tparam RandomIt  std::random_access_iterator
/// \param  first     Beginning of the source range
/// \return Accessory address
template<std::random_access_iterator RandomIt>
requires(sizeof(std::iter_value_t<RandomIt>) == 1uz)
constexpr uint16_t big_endian_data2accessory_address(RandomIt first) {
  return static_cast<uint16_t>(big_endian_data2uint16(first) & 0x07FFu);
}

/// Stupid data to accessory address
///
/// \warning
/// This encoding is used by the LAN_X_CV_POM_ACCESSORY_* commands. I don't know
/// WHAT THE FUCK they where thinking.
///
/// \tparam RandomIt  std::random_access_iterator
/// \param  first     Beginning of the source range
/// \return Accessory address
template<std::random_access_iterator RandomIt>
requires(sizeof(std::iter_value_t<RandomIt>) == 1uz)
constexpr uint16_t stupid_data2accessory_address(RandomIt first) {
  return static_cast<uint16_t>(
    (static_cast<uint32_t>(first[0uz]) << 6u |
     (static_cast<uint32_t>(first[1uz]) & 0xF0u) >> 2u |
     (static_cast<uint32_t>(first[1uz]) & 0x03u)) &
    0x07FFu);
}

/// Little endian data to loco address
///
/// \tparam RandomIt  std::random_access_iterator
/// \param  first     Beginning of the source range
/// \return Loco address
template<std::random_access_iterator RandomIt>
requires(sizeof(std::iter_value_t<RandomIt>) == 1uz)
constexpr uint16_t little_endian_data2loco_address(RandomIt first) {
  return static_cast<uint16_t>(little_endian_data2uint16(first) & 0x3FFFu);
}

} // namespace z21
