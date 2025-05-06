// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Programming interface
///
/// \file   z21/server/intf/programming.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

namespace z21::server::intf {

/// \todo document
///
/// Commands:
/// - LAN_X_DCC_READ_REGISTER (n.a.)
/// - LAN_X_CV_READ
/// - LAN_X_DCC_WRITE_REGISTER (n.a.)
/// - LAN_X_CV_WRITE
/// - LAN_X_MM_WRITE_BYTE (n.a.)
/// - LAN_X_CV_POM (WRITE_BYTE, WRITE_BIT, READ_BYTE)
/// - LAN_X_CV_POM_ACCESSORY (WRITE_BYTE, WRITE_BIT, READ_BYTE)
///
/// Replies:
/// - LAN_X_CV_NACK_SC
/// - LAN_X_CV_NACK
/// - LAN_X_CV_RESULT
struct Programming {
  /// Dtor
  virtual ~Programming() = default;

  // Required

  /// \todo
  [[nodiscard]] virtual bool cvRead(uint16_t cv_addr) = 0;

  /// \todo
  [[nodiscard]] virtual bool cvWrite(uint16_t cv_addr, uint8_t byte) = 0;

  /// \todo
  virtual void cvPomRead(uint16_t loco_addr, uint16_t cv_addr) = 0;

  /// \todo
  virtual void
  cvPomWrite(uint16_t loco_addr, uint16_t cv_addr, uint8_t byte) = 0;

  /// \todo
  virtual void cvPomAccessoryRead(uint16_t accy_addr, uint16_t cv_addr) = 0;

  /// \todo
  virtual void
  cvPomAccessoryWrite(uint16_t accy_addr, uint16_t cv_addr, uint8_t byte) = 0;

  // Implemented by Base

  /// \todo
  virtual void cvNackShortCircuit() = 0;

  /// \todo
  virtual void cvNack() = 0;

  /// \todo
  virtual void cvAck(uint16_t cv_addr, uint8_t byte) = 0;
};

} // namespace z21::server::intf
