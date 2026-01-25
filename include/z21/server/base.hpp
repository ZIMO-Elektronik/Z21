// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Server base
///
/// \file   z21/server/base.hpp
/// \author Vincent Hamp
/// \date   04/11/2023

#pragma once

#include <bit>
#include <map>
#include <span>
#include <ztl/string.hpp>
#include "../broadcast_flags.hpp"
#include "../datasets.hpp"
#include "../db0.hpp"
#include "../exor.hpp"
#include "../hw_type.hpp"
#include "../railcom_data.hpp"
#include "../socket.hpp"
#include "../system_state.hpp"
#include "../utility.hpp"
#include "../x_header.hpp"
#include "intf/interface.hpp"

namespace z21::server {

/// Workaround for Apple Clang
#if defined(__apple_build_version__) || defined(__clang__)
#  define Z21_REQUIRES(...)
#  define Z21_REQUIRE_BODY(CONDITION, MESSAGE)                                 \
    do {                                                                       \
      if constexpr (!(CONDITION)) {                                            \
        static_assert((CONDITION), MESSAGE);                                   \
        return;                                                                \
      }                                                                        \
    } while (false)
#else
#  define Z21_REQUIRES(...) requires(__VA_ARGS__)
#  define Z21_REQUIRE_BODY(CONDITION, MESSAGE)                                 \
    do {                                                                       \
    } while (false)
#endif

template<intf::Interface... Ts>
requires ztl::contains_type_v<intf::System, std::tuple<Ts...>>
class Base : public Ts... {
  struct Client {
    Datasets datasets{};
    BroadcastFlags bc_flags{BroadcastFlags::Default};
    ztl::inplace_deque<uint16_t, Z21_SERVER_MAX_LOCO_ADDRESSES_PER_CLIENT>
      sub_loco_addrs{};
  };

  using Clients = std::map<Socket, Client>;

public:
  /// Ctor
  explicit constexpr Base(SystemState initial_sys_state = {},
                          HwType hw_type = HwType::Z21_NEW)
    : intf::System{initial_sys_state}, _hw_type{hw_type} {}

  /// Dtor
  virtual constexpr ~Base() = default;

  /// \todo document
  void receive(Socket const& sock, std::span<uint8_t const> payload) {
    auto& datasets{_clients[sock].datasets};

    auto first{cbegin(payload)};
    auto const last{cend(payload)};
    while (last - first >= 4 && !full(datasets)) {
      auto const len{first[0uz]};

      // Payload might be zero padded
      // (e.g. entering the "steering" mode of the app does trigger padding)
      if (!len || len > Z21_MAX_DATASET_SIZE)
        return logf('C', sock, "Length out of range");

      auto& [header, chunk]{*end(datasets)};
      header = static_cast<Header>(first[2uz]);
      std::ranges::copy_n(
        first + 4,
        static_cast<std::iter_difference_t<decltype(chunk)>>(len - 4uz),
        begin(chunk));
      chunk.resize(static_cast<decltype(chunk)::size_type>(len - 4uz));
      datasets.push_back();
      first += len;
    }
  }

  /// \todo document
  void execute() {
    for (auto it{begin(_clients)}; it != end(_clients);) it = execute(it);
  }

  /// \todo document
  Clients const& clients() { return _clients; }

  /// \todo document
  void broadcastTurnoutInfo(uint16_t accy_addr) // final
    Z21_REQUIRES(std::derived_from<Base, intf::Driving>) {
    Z21_REQUIRE_BODY((std::derived_from<Base, intf::Driving>),
                     "broadcastTurnoutInfo requires Driving interface");
    lanXTurnoutInfo(accy_addr);
  }

  /// \todo document
  void broadcastExtAccessoryInfo(uint16_t accy_addr) // final
    Z21_REQUIRES(std::derived_from<Base, intf::Driving>) {
    Z21_REQUIRE_BODY((std::derived_from<Base, intf::Driving>),
                     "broadcastExtAccessoryInfo requires Driving interface");
    lanXExtAccessoryInfo(accy_addr);
  }

  /// Implicitly sets the following CentralState flags
  /// - TrackVoltageOff
  void broadcastTrackPowerOff() final { lanXBcTrackPowerOff(); }

  /// Implicitly clears the following CentralState flags
  /// - EmergencyStop
  /// - TrackVoltageOff
  /// - ShortCircuit
  /// - ProgrammingModeActive
  void broadcastTrackPowerOn() final { lanXBcTrackPowerOn(); }

  /// Implicitly sets the following CentralState flags
  /// - ProgrammingModeActive
  void broadcastProgrammingMode() final { lanXBcProgrammingMode(); }

  /// Implicitly sets the following CentralState flags
  /// - ShortCircuit
  void broadcastTrackShortCircuit() final { lanXBcTrackShortCircuit(); }

  /// Implicitly sets the following CentralState flags
  /// - EmergencyStop
  void broadcastStopped() final { lanXBcStopped(); }

  /// \todo document
  void broadcastLocoInfo(uint16_t loco_addr) // final
    Z21_REQUIRES(std::derived_from<Base, intf::Driving>) {
    Z21_REQUIRE_BODY((std::derived_from<Base, intf::Driving>),
                     "broadcastLocoInfo requires Driving interface");
    lanXLocoInfo(loco_addr);
  }

  /// \todo document
  void broadcastSystemStateData() final { lanSystemStateDataChanged(); }

  /// \todo document
  void broadcastRailComData(uint16_t loco_addr) // final
    Z21_REQUIRES(std::derived_from<Base, intf::RailCom>) {
    Z21_REQUIRE_BODY((std::derived_from<Base, intf::RailCom>),
                     "broadcastRailComData requires RailCom interface");
    lanRailComDataChanged(loco_addr);
  }

  /// \todo document
  void cvNackShortCircuit() // final
    Z21_REQUIRES(std::derived_from<Base, intf::Programming>) {
    Z21_REQUIRE_BODY((std::derived_from<Base, intf::Programming>),
                     "cvNackShortCircuit requires Programming interface");
    if (empty(_cv_request_deque)) return;
    lanXCvNackSc(*_cv_request_deque.front());
    _cv_request_deque.pop_front();
  }

  /// \todo document
  void cvNack() // final
    Z21_REQUIRES(std::derived_from<Base, intf::Programming>) {
    Z21_REQUIRE_BODY((std::derived_from<Base, intf::Programming>),
                     "cvNack requires Programming interface");
    if (empty(_cv_request_deque)) return;
    lanXCvNack(*_cv_request_deque.front());
    _cv_request_deque.pop_front();
  }

  /// \todo document
  void cvAck(uint16_t cv_addr, uint8_t byte) // final
    Z21_REQUIRES(std::derived_from<Base, intf::Programming>) {
    Z21_REQUIRE_BODY((std::derived_from<Base, intf::Programming>),
                     "cvAck requires Programming interface");
    if (empty(_cv_request_deque)) return;
    lanXCvResult(*_cv_request_deque.front(), cv_addr, byte);
    _cv_request_deque.pop_front();
  }

private:
  /// Mandatory
  virtual void transmit(Socket const& sock,
                        std::span<uint8_t const> payload) = 0;

  /// \todo document
  void lanGetSerialNumber(Socket const& sock) {
    replyToLanGetSerialNumber(sock);
  }

  /// \todo document
  void lanGetCommonSettings(Socket const& sock)
    requires(std::derived_from<Base, intf::Settings>)
  {
    replyToLanGetCommonSettings(sock);
  }

  /// \todo document
  void lanSetCommonSettings(Socket const&,
                            CommonSettings const& common_settings)
    requires(std::derived_from<Base, intf::Settings>)
  {
    this->commonSettings(common_settings);
  }

  /// \todo document
  void lanGetMmDccSettings(Socket const& sock)
    requires(std::derived_from<Base, intf::Settings>)
  {
    replyToLanGetMmDccSettings(sock);
  }

  /// \todo document
  void lanSetMmDccSettings(Socket const&, MmDccSettings const& mm_dcc_settings)
    requires(std::derived_from<Base, intf::Settings>)
  {
    this->mmDccSettings(mm_dcc_settings);
  }

  /// \todo document
  void lanGetCode(Socket const& sock) { replyToLanGetCode(sock); }

  /// \todo document
  void lanGetHwInfo(Socket const& sock) { replyToLanGetHwInfo(sock); }

  /// \todo document
  ///
  /// \param  sock  Socket (copy!)
  Clients::iterator lanLogoff(Socket sock, Clients::iterator it) {
    it = _clients.erase(it);
    this->logoff(sock);
    return it;
  }

  /// \todo document
  void lanXGetVersion(Socket const& sock) { replyToLanXGetVersion(sock); }

  /// \todo document
  void lanXGetStatus(Socket const& sock) { lanXStatusChanged(sock); }

  /// \todo document
  void lanXSetTrackPowerOff(Socket const& sock) {
    if (this->trackPower(false)) lanXBcTrackPowerOff(sock);
  }

  /// \todo document
  void lanXSetTrackPowerOn(Socket const& sock) {
    if (this->trackPower(true)) lanXBcTrackPowerOn(sock);
  }

  /// \todo document
  void lanXDccReadRegister(Socket const&)
    requires(std::derived_from<Base, intf::Programming>)
  {
    /// \todo
  }

  /// \todo document
  void lanXCvRead(Socket const& sock, uint16_t cv_addr)
    requires(std::derived_from<Base, intf::Programming>)
  {
    if (full(_cv_request_deque)) return;
    else if (_cv_request_deque.push_back(&sock); this->cvRead(cv_addr))
      lanXBcProgrammingMode();
    else _cv_request_deque.pop_back();
  }

  /// \todo document
  void lanXDccWriteRegister(Socket const&)
    requires(std::derived_from<Base, intf::Programming>)
  {
    /// \todo
  }

  /// \todo document
  void lanXCvWrite(Socket const& sock, uint16_t cv_addr, uint8_t byte)
    requires(std::derived_from<Base, intf::Programming>)
  {
    if (full(_cv_request_deque)) return;
    else if (_cv_request_deque.push_back(&sock); this->cvWrite(cv_addr, byte))
      lanXBcProgrammingMode();
    else _cv_request_deque.pop_back();
  }

  /// \todo document
  void lanXMmWriteByte(Socket const&)
    requires(std::derived_from<Base, intf::Programming>)
  {
    /// \todo
  }

  /// \todo document
  void lanXGetTurnoutInfo(Socket const& sock, uint16_t accy_addr)
    requires(std::derived_from<Base, intf::Switching>)
  {
    lanXTurnoutInfo(accy_addr, sock);
  }

  /// \todo document
  void lanXGetExtAccessoryInfo(Socket const& sock, uint16_t accy_addr)
    requires(std::derived_from<Base, intf::Switching>)
  {
    lanXExtAccessoryInfo(accy_addr, sock);
  }

  /// \todo document
  void lanXSetTurnout(Socket const&, uint16_t accy_addr, bool p, bool a, bool q)
    requires(std::derived_from<Base, intf::Switching>)
  {
    this->turnout(accy_addr, p, a, q);
  }

  /// \todo document
  void lanXSetExtAccessory(Socket const&, uint16_t accy_addr, uint8_t dddddddd)
    requires(std::derived_from<Base, intf::Switching>)
  {
    this->accessory(accy_addr, dddddddd);
  }

  /// \todo document
  void lanXSetStop(Socket const& sock) {
    if (this->stop()) lanXBcStopped(sock);
  }

  /// \todo document
  void lanXSetLocoEStop(Socket const&, uint16_t loco_addr)
    requires(std::derived_from<Base, intf::Driving>)
  {
    auto const loco_info{this->locoInfo(loco_addr)};
    this->locoDrive(loco_addr,
                    loco_info.speed_steps,
                    (loco_info.rvvvvvvv & 0b1000'0000u) | 0b1u);
  }

  /// \todo document
  void lanXPurgeLoco(Socket const&, uint16_t)
    requires(std::derived_from<Base, intf::Driving>)
  {
    /// \todo
  }

  /// \todo document
  void lanXGetLocoInfo(Socket const& sock, uint16_t loco_addr)
    requires(std::derived_from<Base, intf::Driving>)
  {
    // Subscribe if broadcast flag is set and not already done so
    if (auto& loco_addrs{_clients[sock].sub_loco_addrs};
        std::to_underlying(_clients[sock].bc_flags &
                           BroadcastFlags::DrivingSwitching) &&
        !std::ranges::contains(loco_addrs, loco_addr)) {
      if (full(loco_addrs)) loco_addrs.pop_front(); // FIFO full, replace first
      loco_addrs.push_back(loco_addr);
    }

    lanXLocoInfo(loco_addr, sock);
  }

  /// \todo document
  void lanXSetLocoDrive(Socket const&,
                        uint16_t loco_addr,
                        LocoInfo::SpeedSteps speed_steps,
                        uint8_t rvvvvvvv)
    requires(std::derived_from<Base, intf::Driving>)
  {
    this->locoDrive(loco_addr, speed_steps, rvvvvvvv);
  }

  /// \todo document
  void lanXSetLocoFunction(Socket const&,
                           uint16_t loco_addr,
                           uint8_t state,
                           uint8_t index)
    requires(std::derived_from<Base, intf::Driving>)
  {
    assert(state < 0b11u);
    switch (state) {
      // Off
      case 0b00u: [[fallthrough]];
      // On
      case 0b01u:
        return this->locoFunction(
          loco_addr, 1u << index, static_cast<uint32_t>(state << index));
      // Toggle
      case 0b10u: {
        auto const loco_info{this->locoInfo(loco_addr)};
        return this->locoFunction(
          loco_addr,
          1u << index,
          static_cast<uint32_t>(
            !(static_cast<bool>(loco_info.f31_0 & (1u << index))) << index));
        break;
      }
    }
  }

  /// \todo document
  void lanXSetLocoFunctionGroup(Socket const&,
                                uint16_t loco_addr,
                                uint8_t group,
                                uint8_t state)
    requires(std::derived_from<Base, intf::Driving>)
  {
    switch (static_cast<DB0>(group)) {
      case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_1:
        return this->locoFunction(
          loco_addr, 0b1'1111u, (state & 0b0'1111u) << 1u | state >> 4u);
      case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_2:
        return this->locoFunction(
          loco_addr, 0b1111u << 5u, static_cast<uint32_t>(state << 5u));
      case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_3:
        return this->locoFunction(
          loco_addr, 0b1111u << 9u, static_cast<uint32_t>(state << 9u));
      case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_4:
        return this->locoFunction(
          loco_addr, 0b1111u << 13u, static_cast<uint32_t>(state << 13u));
      case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_5:
        return this->locoFunction(
          loco_addr, 0b1111u << 21u, static_cast<uint32_t>(state << 21u));
      case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_6: break;
      case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_7: break;
      case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_8: break;
      case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_9: break;
      case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_10: break;
      default: break;
    }
  }

  /// \todo document
  void lanXSetLocoBinaryState(Socket const&)
    requires(std::derived_from<Base, intf::Driving>)
  {
    /// \todo
  }

  /// \todo document
  void lanXCvPomWriteByte(Socket const&,
                          uint16_t loco_addr,
                          uint16_t cv_addr,
                          uint8_t byte)
    requires(std::derived_from<Base, intf::Programming>)
  {
    this->cvPomWrite(loco_addr, cv_addr, byte);
  }

  /// \todo document
  void lanXCvPomWriteBit(Socket const&)
    requires(std::derived_from<Base, intf::Programming>)
  {
    assert(false); // Only implement this if really necessary
  }

  /// \todo document
  void
  lanXCvPomReadByte(Socket const& sock, uint16_t loco_addr, uint16_t cv_addr)
    requires(std::derived_from<Base, intf::Programming>)
  {
    if (full(_cv_request_deque)) return;
    _cv_request_deque.push_back(&sock);
    this->cvPomRead(loco_addr, cv_addr);
  }

  /// \todo document
  void lanXCvPomAccessoryWriteByte(
    Socket const&, uint16_t accy_addr, uint16_t cv_addr, uint8_t byte, bool c)
    requires(std::derived_from<Base, intf::Programming>)
  {
    this->cvPomAccessoryWrite(accy_addr, cv_addr, byte, c);
  }

  /// \todo document
  void lanXCvPomAccessoryWriteBit(Socket const&)
    requires(std::derived_from<Base, intf::Programming>)
  {
    assert(false); // Only implement this if really necessary
  }

  /// \todo document
  void lanXCvPomAccessoryReadByte(Socket const& sock,
                                  uint16_t accy_addr,
                                  uint16_t cv_addr,
                                  bool c)
    requires(std::derived_from<Base, intf::Programming>)
  {
    if (full(_cv_request_deque)) return;
    _cv_request_deque.push_back(&sock);
    this->cvPomAccessoryRead(accy_addr, cv_addr, c);
  }

  /// \todo document
  /// https://www.z21.eu/en/products/z21-maintenance-tool
  void lanXGetFirmwareVersion(Socket const& sock) {
    replyToLanXGetFirmwareVersion(sock);
  }

  /// \todo document
  void lanSetBroadcastFlags(Socket const& sock, BroadcastFlags flags) {
    // print_broadcast_flags(flags);
    _clients[sock].bc_flags = flags;
  }

  /// \todo document
  void lanGetBroadcastFlags(Socket const& sock) {
    replyToLanGetBroadcastFlags(sock);
  }

  /// \todo document
  void lanGetLocoMode(Socket const& sock, uint16_t loco_addr)
    requires(std::derived_from<Base, intf::Driving>)
  {
    replyToLanGetLocoMode(sock, loco_addr);
  }

  /// \todo document
  void lanSetLocoMode(Socket const&, uint16_t loco_addr, LocoInfo::Mode mode)
    requires(std::derived_from<Base, intf::Driving>)
  {
    this->locoMode(loco_addr, mode);
  }

  /// \todo document
  void lanGetTurnoutMode(Socket const& sock, uint16_t accy_addr)
    requires(std::derived_from<Base, intf::Switching>)
  {
    replyToLanGetTurnoutMode(sock, accy_addr);
  }

  /// \todo document
  void
  lanSetTurnoutMode(Socket const&, uint16_t accy_addr, TurnoutInfo::Mode mode)
    requires(std::derived_from<Base, intf::Switching>)
  {
    this->turnoutMode(accy_addr, mode);
  }

  /// \todo document
  void lanRmBusGetData(Socket const&)
    requires(std::derived_from<Base, intf::RBus>)
  {
    /// \todo
  }

  /// \todo document
  void lanRmBusProgramModule(Socket const&)
    requires(std::derived_from<Base, intf::RBus>)
  {
    /// \todo
  }

  /// \todo document
  void lanSystemStateGetData(Socket const& sock) {
    lanSystemStateDataChanged(sock);
  }

  /// \todo document
  void lanRailComGetData(Socket const& sock, uint8_t type, uint16_t loco_addr)
    requires(std::derived_from<Base, intf::RailCom>)
  {
    /// \bug https://github.com/ZIMO-Elektronik/Z21/issues/13
    assert(type == 0x01u);
    lanRailComDataChanged(loco_addr, sock);
  }

  /// \todo document
  void lanLocoNetFromLan(Socket const&)
    requires(std::derived_from<Base, intf::LocoNet>)
  {
    /// \todo
  }

  /// \todo document
  void lanLocoNetDispatchAddr(Socket const&)
    requires(std::derived_from<Base, intf::LocoNet>)
  {
    /// \todo
  }

  /// \todo document
  void lanLocoNetDetector(Socket const&)
    requires(std::derived_from<Base, intf::LocoNet>)
  {
    /// \todo
  }

  /// \todo document
  void lanCanDetector(Socket const&)
    requires(std::derived_from<Base, intf::Can>)
  {
    /// \todo
  }

  /// \todo document
  void lanCanDeviceGetDescription(Socket const&)
    requires(std::derived_from<Base, intf::Can>)
  {
    /// \todo
  }

  /// \todo document
  void lanCanDeviceSetDescription(Socket const&)
    requires(std::derived_from<Base, intf::Can>)
  {
    /// \todo
  }

  /// \todo document
  void lanCanBoosterSetTrackPower(Socket const&)
    requires(std::derived_from<Base, intf::Can>)
  {
    /// \todo
  }

  /// \todo document
  void lanFastClockControl(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanFastClockSettingsGet(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanFastClockSettingsSet(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanBoosterSetPower(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanBoosterGetDescription(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanBoosterSetDescription(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanBoosterSystemStateGetData(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanDecoderGetDescription(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanDecoderSetDescription(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanDecoderSystemStateGetData(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanZLinkGetHwInfo(Socket const&)
    requires(std::derived_from<Base, intf::ZLink>)
  {
    /// \todo
  }

  /// <<< REPLIES START HERE >>>

  /// \todo document
  void replyToLanGetSerialNumber(Socket const& sock) {
    auto const sn{this->serialNumber()};
    if (!sn) return;
    std::array<uint8_t, 0x08uz> const reply{
      0x08u,                                                      // Length
      0x00u,                                                      //
      std::to_underlying(Header::Reply_to_LAN_GET_SERIAL_NUMBER), // Header
      0x00u,                                                      //
      static_cast<uint8_t>(sn >> 0u),   // Serial number
      static_cast<uint8_t>(sn >> 8u),   //
      static_cast<uint8_t>(sn >> 16u),  //
      static_cast<uint8_t>(sn >> 24u)}; //
    this->transmit(sock, reply);
    logf('S', sock, "Reply_to_LAN_GET_SERIAL_NUMBER", reply);
  }

  /// \todo document
  void replyToLanGetCommonSettings(Socket const& sock)
    requires(std::derived_from<Base, intf::Settings>)
  {
    auto const common_settings{this->commonSettings()};
    std::array<uint8_t, 0x0Euz> const reply{
      0x0Eu,                                                        // Length
      0x00u,                                                        //
      std::to_underlying(Header::Reply_to_LAN_GET_COMMON_SETTINGS), // Header
      0x00u,                                                        //
      common_settings.enable_railcom,                               //
      common_settings.enable_bit_modify_on_long_address,            //
      common_settings.key_stop_mode,                                //
      common_settings.programming_type,                             //
      common_settings.enable_loconet_current_source,                //
      common_settings.loconet_fast_clock_rate,                      //
      common_settings.loconet_mode,                                 //
      common_settings.ext_settings,                                 //
      common_settings.purging_time,                                 //
      common_settings.bus_settings};                                //
    this->transmit(sock, reply);
    logf('S', sock, "Reply_to_LAN_GET_COMMON_SETTINGS", reply);
  }

  /// \todo document
  void replyToLanGetMmDccSettings(Socket const& sock)
    requires(std::derived_from<Base, intf::Settings>)
  {
    auto const mm_dcc_settings{this->mmDccSettings()};
    std::array<uint8_t, 0x14uz> const reply{
      0x14u,                                                           // Length
      0x00u,                                                           //
      std::to_underlying(Header::Reply_to_LAN_GET_MMDCC_SETTINGS),     // Header
      0x00u,                                                           //
      mm_dcc_settings.startup_reset_package_count,                     //
      mm_dcc_settings.continue_reset_packet_count,                     //
      mm_dcc_settings.program_package_count,                           //
      mm_dcc_settings.bit_verify_to_one,                               //
      0x00u,                                                           //
      0x00u,                                                           //
      0x00u,                                                           //
      0x00u,                                                           //
      0x00u,                                                           //
      0x00u,                                                           //
      mm_dcc_settings.programming_ack_current,                         //
      mm_dcc_settings.flags,                                           //
      static_cast<uint8_t>(mm_dcc_settings.output_voltage >> 0u),      //
      static_cast<uint8_t>(mm_dcc_settings.output_voltage >> 8u),      //
      static_cast<uint8_t>(mm_dcc_settings.programming_voltage >> 0u), //
      static_cast<uint8_t>(mm_dcc_settings.programming_voltage >> 8u)}; //
    this->transmit(sock, reply);
    logf('S', sock, "Reply_to_LAN_GET_MMDCC_SETTINGS", reply);
  }

  /// \todo document
  void replyToLanGetCode(Socket const& sock) {
    static constexpr std::array<uint8_t, 0x05uz> reply{
      0x08u,                                             // Length
      0x00u,                                             //
      std::to_underlying(Header::Reply_to_LAN_GET_CODE), // Header
      0x00u,                                             //
      0x00u};                                            // Code
    this->transmit(sock, reply);
    logf('S', sock, "Reply_to_LAN_GET_CODE", reply);
  }

  /// \todo document
  void replyToLanGetHwInfo(Socket const& sock) {
    std::array<uint8_t, 0x0Cuz> const reply{
      0x0Cu,                                                     // Length
      0x00u,                                                     //
      std::to_underlying(Header::Reply_to_LAN_GET_HWINFO),       // Header
      0x00u,                                                     //
      static_cast<uint8_t>(std::to_underlying(_hw_type) >> 0u),  // HW type
      static_cast<uint8_t>(std::to_underlying(_hw_type) >> 8u),  //
      static_cast<uint8_t>(std::to_underlying(_hw_type) >> 16u), //
      static_cast<uint8_t>(std::to_underlying(_hw_type) >> 24u), //
      Z21_FIRMWARE_VERSION_MINOR,                                // FW version
      Z21_FIRMWARE_VERSION_MAJOR,                                //
      0x00u,                                                     //
      0x00u};                                                    //
    this->transmit(sock, reply);
    logf('S', sock, "Reply_to_LAN_GET_HWINFO", reply);
  }

  /// document \todo
  ///
  /// This message is sent from the Z21 to the clients in response to the
  /// command LAN_X_GET_TURNOUT_INFO. However, it is also sent to an associated
  /// client unsolicitedly if
  /// - the function status has been changed by one of the (other) clients or a
  ///   handset controller
  /// - and the associated client has activated the corresponding broadcast
  void lanXTurnoutInfo(uint16_t accy_addr, Socket const& sock = {})
    requires(std::derived_from<Base, intf::Switching>)
  {
    auto const turnout_info{this->turnoutInfo(accy_addr)};

    std::array<uint8_t, 0x09uz> reply{
      0x09u,                                           // Length
      0x00u,                                           //
      std::to_underlying(Header::LAN_X_TURNOUT_INFO),  // Header
      0x00u,                                           //
      std::to_underlying(XHeader::LAN_X_TURNOUT_INFO), // X-Header
      static_cast<uint8_t>((accy_addr >> 8u) & 0x07u), // DB0
      static_cast<uint8_t>(accy_addr),                 // DB1
      static_cast<uint8_t>(turnout_info.position),     // DB2
      0x00u};                                          // XOR
    reply.back() = exor({cbegin(reply) + 4, cend(reply) - 1});

    //
    if (sock) {
      this->transmit(sock, reply);
      logf('S', sock, "LAN_X_TURNOUT_INFO", reply);
    }
    //
    else
      for (auto const& [s, c] : _clients)
        if (std::to_underlying(c.bc_flags & BroadcastFlags::DrivingSwitching)) {
          this->transmit(s, reply);
          logf('S', s, "LAN_X_TURNOUT_INFO", reply);
        }
  }

  /// \todo document
  ///
  /// This message is sent from the Z21 to the clients in response to command
  /// LAN_X_GET_EXT_ACCESSORY_INFO. However, it is also sent to an associated
  /// client unsolicitedly if
  /// - the accessory status has been changed by one of the (other) clients or a
  ///   handset controller
  /// - and the associated client has activated the corresponding broadcast
  void lanXExtAccessoryInfo(uint16_t accy_addr, Socket const& sock = {})
    requires(std::derived_from<Base, intf::Switching>)
  {
    auto const accessory_info{this->accessoryInfo(accy_addr)};

    std::array<uint8_t, 0x0Auz> reply{
      0x0Auz,                                                // Length
      0x00u,                                                 //
      std::to_underlying(Header::LAN_X_EXT_ACCESSORY_INFO),  // Header
      0x00u,                                                 //
      std::to_underlying(XHeader::LAN_X_EXT_ACCESSORY_INFO), // X-Header
      static_cast<uint8_t>((accy_addr >> 8u) & 0x07u),       // DB0
      static_cast<uint8_t>(accy_addr),                       // DB1
      static_cast<uint8_t>(accessory_info.dddddddd),         // DB2
      static_cast<uint8_t>(accessory_info.status),           // DB3
      0x00u};                                                // XOR
    reply.back() = exor({cbegin(reply) + 4, cend(reply) - 1});

    //
    if (sock) {
      this->transmit(sock, reply);
      logf('S', sock, "LAN_X_EXT_ACCESSORY_INFO", reply);
    }
    //
    else
      for (auto const& [s, c] : _clients)
        if (std::to_underlying(c.bc_flags & BroadcastFlags::DrivingSwitching)) {
          this->transmit(s, reply);
          logf('S', s, "LAN_X_EXT_ACCESSORY_INFO", reply);
        }
  }

  /// \todo document
  ///
  /// The following packet is sent from the Z21 to the registered clients when
  /// - a client has sent command LAN_X_SET_TRACK_POWER_OFF
  /// - or the track voltage has been switched off by some input device
  /// - and the relevant client has activated the corresponding broadcast
  void lanXBcTrackPowerOff(Socket const& sock = {}) {
    //
    intf::System::systemState().central_state |= CentralState::TrackVoltageOff;

    //
    _cv_request_deque.clear();

    static constexpr std::array<uint8_t, 0x07uz> reply{
      0x07u,                                                 // Length
      0x00u,                                                 //
      std::to_underlying(Header::LAN_X_BC_TRACK_POWER_OFF),  // Header
      0x00u,                                                 //
      std::to_underlying(XHeader::LAN_X_BC_TRACK_POWER_OFF), // X-Header
      std::to_underlying(DB0::LAN_X_BC_TRACK_POWER_OFF),     // DB0
      0x61u};                                                // XOR

    for (auto const& [s, c] : _clients)
      if (s == sock ||
          std::to_underlying(c.bc_flags & BroadcastFlags::DrivingSwitching)) {
        this->transmit(s, reply);
        logf('S', s, "LAN_X_BC_TRACK_POWER_OFF", reply);
      }
  }

  /// \todo document
  ///
  /// The following packet is sent from the Z21 to the registered clients when
  /// - a client has sent command LAN_X_SET_TRACK_POWER_ON
  /// - or the track voltage has been switched on by some input device
  /// - and the relevant client has activated the corresponding broadcast
  void lanXBcTrackPowerOn(Socket const& sock = {}) {
    //
    intf::System::systemState().central_state &=
      ~(CentralState::EmergencyStop |         //
        CentralState::TrackVoltageOff |       //
        CentralState::ShortCircuit |          //
        CentralState::ProgrammingModeActive); //

    //
    _cv_request_deque.clear();

    static constexpr std::array<uint8_t, 0x07uz> reply{
      0x07u,                                                // Length
      0x00u,                                                //
      std::to_underlying(Header::LAN_X_BC_TRACK_POWER_ON),  // Header
      0x00u,                                                //
      std::to_underlying(XHeader::LAN_X_BC_TRACK_POWER_ON), // X-Header
      std::to_underlying(DB0::LAN_X_BC_TRACK_POWER_ON),     // DB0
      0x60u};                                               // XOR

    for (auto const& [s, c] : _clients)
      if (s == sock ||
          std::to_underlying(c.bc_flags & BroadcastFlags::DrivingSwitching)) {
        this->transmit(s, reply);
        logf('S', s, "LAN_X_BC_TRACK_POWER_ON", reply);
      }
  }

  /// \todo document
  ///
  /// The following packet is sent from the Z21 to the registered clients if the
  /// Z21 has been put into CV programming mode by LAN_X_CV_READ or
  /// LAN_X_CV_WRITE and the respective client has activated the corresponding
  /// broadcast.
  void lanXBcProgrammingMode(Socket const& sock = {}) {
    intf::System::systemState().central_state |=
      CentralState::ProgrammingModeActive;

    static constexpr std::array<uint8_t, 0x07uz> reply{
      0x07u,                                                  // Length
      0x00u,                                                  //
      std::to_underlying(Header::LAN_X_BC_PROGRAMMING_MODE),  // Header
      0x00u,                                                  //
      std::to_underlying(XHeader::LAN_X_BC_PROGRAMMING_MODE), // X-Header
      std::to_underlying(DB0::LAN_X_BC_PROGRAMMING_MODE),     // DB0
      0x63u};                                                 // XOR

    for (auto const& [s, c] : _clients)
      if (s == sock ||
          std::to_underlying(c.bc_flags & BroadcastFlags::DrivingSwitching)) {
        this->transmit(s, reply);
        logf('S', s, "LAN_X_BC_PROGRAMMING_MODE", reply);
      }
  }

  /// \todo document
  ///
  /// The following packet is sent from the Z21 to the registered clients if a
  /// short circuit has occurred and the relevant client has activated the
  /// corresponding broadcast.
  void lanXBcTrackShortCircuit() {
    intf::System::systemState().central_state |= CentralState::ShortCircuit;

    static constexpr std::array<uint8_t, 0x07uz> reply{
      0x07u,                                                     // Length
      0x00u,                                                     //
      std::to_underlying(Header::LAN_X_BC_TRACK_SHORT_CIRCUIT),  // Header
      0x00u,                                                     //
      std::to_underlying(XHeader::LAN_X_BC_TRACK_SHORT_CIRCUIT), // X-Header
      std::to_underlying(DB0::LAN_X_BC_TRACK_SHORT_CIRCUIT),     // DB0
      0x69u};                                                    // XOR

    for (auto const& [s, c] : _clients)
      if (std::to_underlying(c.bc_flags & BroadcastFlags::DrivingSwitching)) {
        this->transmit(s, reply);
        logf('S', s, "LAN_X_BC_TRACK_SHORT_CIRCUIT", reply);
      }
  }

  /// \todo document
  void lanXCvNackSc(Socket const& sock)
    requires(std::derived_from<Base, intf::Programming>)
  {
    static constexpr std::array<uint8_t, 0x07uz> reply{
      0x07u,                                         // Length
      0x00u,                                         //
      std::to_underlying(Header::LAN_X_CV_NACK_SC),  // Header
      0x00u,                                         //
      std::to_underlying(XHeader::LAN_X_CV_NACK_SC), // X-Header
      std::to_underlying(DB0::LAN_X_CV_NACK_SC),     // DB0
      0x73u};                                        // XOR
    this->transmit(sock, reply);
    logf('S', sock, "LAN_X_CV_NACK_SC", reply);
  }

  /// \todo document
  void lanXCvNack(Socket const& sock)
    requires(std::derived_from<Base, intf::Programming>)
  {
    static constexpr std::array<uint8_t, 0x07uz> reply{
      0x07u,                                      // Length
      0x00u,                                      //
      std::to_underlying(Header::LAN_X_CV_NACK),  // Header
      0x00u,                                      //
      std::to_underlying(XHeader::LAN_X_CV_NACK), // X-Header
      std::to_underlying(DB0::LAN_X_CV_NACK),     // DB0
      0x72u};                                     // XOR
    this->transmit(sock, reply);
    logf('S', sock, "LAN_X_CV_NACK", reply);
  }

  /// \todo document
  ///
  /// The following packet is sent from the Z21 to the client in response to an
  /// invalid request.
  void lanXUnknownCommand(Socket const& sock) {
    static constexpr std::array<uint8_t, 0x07uz> reply{
      0x07u,                                              // Length
      0x00u,                                              //
      std::to_underlying(Header::LAN_X_UNKNOWN_COMMAND),  // Header
      0x00u,                                              //
      std::to_underlying(XHeader::LAN_X_UNKNOWN_COMMAND), // X-Header
      std::to_underlying(DB0::LAN_X_UNKNOWN_COMMAND),     // DB0
      0xE3u};                                             // XOR
    this->transmit(sock, reply);
    logf('S', sock, "LAN_X_UNKNOWN_COMMAND", reply);
  }

  /// \todo document
  void lanXStatusChanged(Socket const& sock) {
    std::array<uint8_t, 0x08uz> reply{
      0x08u,                                                 // Length
      0x00u,                                                 //
      std::to_underlying(Header::LAN_X_STATUS_CHANGED),      // Header
      0x00u,                                                 //
      std::to_underlying(XHeader::LAN_X_STATUS_CHANGED),     // X-Header
      std::to_underlying(DB0::LAN_X_STATUS_CHANGED),         // DB0
      std::to_underlying(this->systemState().central_state), // DB1
      0x00u};                                                // XOR
    reply.back() = exor({cbegin(reply) + 4, cend(reply) - 1});
    this->transmit(sock, reply);
    logf('S', sock, "LAN_X_STATUS_CHANGED", reply);
  }

  /// \todo document
  void replyToLanXGetVersion(Socket const& sock) {
    std::array<uint8_t, 0x09uz> reply{
      0x09u,                                                   // Length
      0x00u,                                                   //
      std::to_underlying(Header::Reply_to_LAN_X_GET_VERSION),  // Header
      0x00u,                                                   //
      std::to_underlying(XHeader::Reply_to_LAN_X_GET_VERSION), // X-Header
      std::to_underlying(DB0::Reply_to_LAN_X_GET_VERSION),     // DB0
      Z21_XBUS_PROTOCOL_VERSION, // X-Bus protocol version
      Z21_COMMAND_STATION_ID,    // Command station ID
      0x00u};                    // XOR
    reply.back() = exor({cbegin(reply) + 4, cend(reply) - 1});
    this->transmit(sock, reply);
    logf('S', sock, "Reply_to_LAN_X_GET_VERSION", reply);
  }

  /// \todo document
  void lanXCvResult(Socket const& sock, uint16_t cv_addr, uint8_t byte)
    requires(std::derived_from<Base, intf::Programming>)
  {
    std::array<uint8_t, 0x0Auz> reply{
      0x0Au,                                        // Length
      0x00u,                                        //
      std::to_underlying(Header::LAN_X_CV_RESULT),  // Header
      0x00u,                                        //
      std::to_underlying(XHeader::LAN_X_CV_RESULT), // X-Header
      std::to_underlying(DB0::LAN_X_CV_RESULT),     // DB0
      static_cast<uint8_t>(cv_addr >> 8u),          // CV address
      static_cast<uint8_t>(cv_addr >> 0u),          //
      byte,                                         // Value
      0x00u};                                       //
    reply.back() = exor({cbegin(reply) + 4, cend(reply) - 1});
    this->transmit(sock, reply);
    logf('S', sock, "LAN_X_CV_RESULT", reply);
  }

  /// \todo document
  ///
  /// The following packet is sent from the Z21 to the registered clients when
  /// - a client has sent command LAN_X_SET_STOP
  /// - or the emergency stop was triggered by some input device
  /// - and the relevant client has activated the corresponding broadcast
  void lanXBcStopped(Socket const& sock = {}) {
    intf::System::systemState().central_state |= CentralState::EmergencyStop;

    static constexpr std::array<uint8_t, 0x07uz> reply{
      0x07u,                                         // Length
      0x00u,                                         //
      std::to_underlying(Header::LAN_X_BC_STOPPED),  // Header
      0x00u,                                         //
      std::to_underlying(XHeader::LAN_X_BC_STOPPED), // X-Header
      0x00u,                                         // DB0
      0x81u};                                        // XOR

    for (auto const& [s, c] : _clients)
      if (s == sock ||
          std::to_underlying(c.bc_flags & BroadcastFlags::DrivingSwitching)) {
        this->transmit(s, reply);
        logf('S', s, "LAN_X_BC_STOPPED", reply);
      }
  }

  /// \todo document
  ///
  /// This message is sent from the Z21 to the clients in response to the
  /// command LAN_X_GET_LOCO_INFO. However, it is also unsolicitedly sent to an
  /// associated client if
  /// - the locomotive status has been changed by one of the (other) clients or
  ///   handset controls
  /// - and the associated client has activated the corresponding broadcast
  /// - and the associated client has subscribed to the locomotive address with
  ///   LAN_X_GET_LOCO_INFO
  void lanXLocoInfo(uint16_t loco_addr, Socket const& sock = {}) {
    auto const loco_info{this->locoInfo(loco_addr)};

    auto const db0{static_cast<uint8_t>((loco_addr >= 128u ? 0xC0u : 0x00u) |
                                        loco_addr >> 8u)};
    auto const db1{static_cast<uint8_t>(loco_addr >> 0u)};
    auto const db2{
      static_cast<uint8_t>(loco_info.mode << 4u | loco_info.busy << 3u |
                           std::to_underlying(loco_info.speed_steps))};
    auto const db3{loco_info.rvvvvvvv};
    auto const db4{
      static_cast<uint8_t>(loco_info.double_traction << 6u |      //
                           loco_info.smart_search << 5u |         //
                           (loco_info.f31_0 & 0b1'1110u) >> 1u |  // F4-F1
                           (loco_info.f31_0 & 0b0'0001u) << 4u)}; // F0

    std::array<uint8_t, 0x0Fuz> reply{
      0x0Fu,                                        // Length
      0x00u,                                        //
      std::to_underlying(Header::LAN_X_LOCO_INFO),  // Header
      0x00u,                                        //
      std::to_underlying(XHeader::LAN_X_LOCO_INFO), // X-Header
      db0,                                          // DB0
      db1,                                          // DB1
      db2,                                          // DB2
      db3,                                          // DB3
      db4,                                          // DB4
      static_cast<uint8_t>(loco_info.f31_0 >> 5u),  // DB5
      static_cast<uint8_t>(loco_info.f31_0 >> 13u), // DB6
      static_cast<uint8_t>(loco_info.f31_0 >> 21u), // DB7
      static_cast<uint8_t>(loco_info.f31_0 >> 29u), // DB8
      0x00u};                                       // XOR
    reply.back() = exor({cbegin(reply) + 4, cend(reply) - 1});

    //
    if (sock) {
      this->transmit(sock, reply);
      logf('S', sock, "LAN_X_LOCO_INFO", reply);
    }
    //
    else
      for (auto const& [s, c] : _clients)
        if (std::to_underlying(c.bc_flags & BroadcastFlags::DrivingSwitching) &&
            std::ranges::contains(c.sub_loco_addrs, loco_addr)) {
          this->transmit(s, reply);
          logf('S', s, "LAN_X_LOCO_INFO", reply);
        }
  }

  /// \todo document
  void replyToLanXGetFirmwareVersion(Socket const& sock) {
    std::array<uint8_t, 0x09uz> reply{
      0x09uz,                                                          // Length
      0x00u,                                                           //
      std::to_underlying(Header::Reply_to_LAN_X_GET_FIRMWARE_VERSION), // Header
      0x00u,                                                           //
      std::to_underlying(
        XHeader::Reply_to_LAN_X_GET_FIRMWARE_VERSION),              // X-Header
      std::to_underlying(DB0::Reply_to_LAN_X_GET_FIRMWARE_VERSION), // DB0
      Z21_FIRMWARE_VERSION_MAJOR, // FW version
      Z21_FIRMWARE_VERSION_MINOR, //
      0x00u};                     // XOR
    reply.back() = exor({cbegin(reply), size(reply) - 1uz});
    this->transmit(sock, reply);
    logf('S', sock, "Reply_to_LAN_X_GET_FIRMWARE_VERSION", reply);
  }

  /// \todo document
  void replyToLanGetBroadcastFlags(Socket const& sock) {
    auto const bc_flags{std::to_underlying(_clients[sock].bc_flags)};
    std::array<uint8_t, 0x08uz> const reply{
      0x08u,                                                       // Length
      0x00u,                                                       //
      std::to_underlying(Header::Reply_to_LAN_GET_BROADCASTFLAGS), // Header
      0x00u,                                                       //
      static_cast<uint8_t>(bc_flags >> 0u),   // Broadcast flags
      static_cast<uint8_t>(bc_flags >> 8u),   //
      static_cast<uint8_t>(bc_flags >> 16u),  //
      static_cast<uint8_t>(bc_flags >> 24u)}; //
    this->transmit(sock, reply);
    logf('S', sock, "Reply_to_LAN_GET_BROADCASTFLAGS", reply);
  }

  /// \todo document
  void replyToLanGetLocoMode(Socket const& sock, uint16_t loco_addr)
    requires(std::derived_from<Base, intf::Driving>)
  {
    auto const mode{this->locoMode(loco_addr)};
    std::array<uint8_t, 0x07uz> const reply{
      0x07u,                                                 // Length
      0x00u,                                                 //
      std::to_underlying(Header::Reply_to_LAN_GET_LOCOMODE), // Header
      0x00u,                                                 //
      static_cast<uint8_t>(loco_addr >> 8u),                 // Address
      static_cast<uint8_t>(loco_addr >> 0u),                 //
      std::to_underlying(mode)};                             // Mode
    this->transmit(sock, reply);
    logf('S', sock, "Reply_to_LAN_GET_LOCOMODE", reply);
  }

  /// \todo document
  void replyToLanGetTurnoutMode(Socket const& sock, uint16_t accy_addr)
    requires(std::derived_from<Base, intf::Switching>)
  {
    auto const mode{this->turnoutMode(accy_addr)};
    std::array<uint8_t, 0x07uz> const reply{
      0x07u,                                                    // Length
      0x00u,                                                    //
      std::to_underlying(Header::Reply_to_LAN_GET_TURNOUTMODE), // Header
      0x00u,                                                    //
      static_cast<uint8_t>(accy_addr >> 8u),                    // Address
      static_cast<uint8_t>(accy_addr >> 0u),                    //
      std::to_underlying(mode)};                                // Mode
    this->transmit(sock, reply);
    logf('S', sock, "Reply_to_LAN_GET_TURNOUTMODE", reply);
  }

  /// \todo document
  void lanRmbusDataChanged(Socket const&) {
    /// \todo
  }

  /// \todo document
  ///
  /// This message is asynchronously reported to the client by the Z21 when the
  /// client
  /// - activated the corresponding broadcast
  /// - explicitly requested the system status
  void lanSystemStateDataChanged(Socket const& sock = {}) {
    auto const sys_state{this->systemState()};

    std::array<uint8_t, 0x14uz> const reply{
      0x14u,                                                       // Length
      0x00u,                                                       //
      std::to_underlying(Header::LAN_SYSTEMSTATE_DATACHANGED),     // Header
      0x00u,                                                       //
      static_cast<uint8_t>(sys_state.main_current >> 0u),          //
      static_cast<uint8_t>(sys_state.main_current >> 8u),          //
      static_cast<uint8_t>(sys_state.prog_current >> 0u),          //
      static_cast<uint8_t>(sys_state.prog_current >> 8u),          //
      static_cast<uint8_t>(sys_state.filtered_main_current >> 0u), //
      static_cast<uint8_t>(sys_state.filtered_main_current >> 8u), //
      static_cast<uint8_t>(sys_state.temperature >> 0u),           //
      static_cast<uint8_t>(sys_state.temperature >> 8u),           //
      static_cast<uint8_t>(sys_state.supply_voltage >> 0u),        //
      static_cast<uint8_t>(sys_state.supply_voltage >> 8u),        //
      static_cast<uint8_t>(sys_state.vcc_voltage >> 0u),           //
      static_cast<uint8_t>(sys_state.vcc_voltage >> 8u),           //
      std::to_underlying(sys_state.central_state),    // CentralState
      std::to_underlying(sys_state.central_state_ex), // CentralStateEx
      0x00u,                                          // Reserved
      std::to_underlying(sys_state.capabilities)};    // Capabilities

    // Transmit to every client with broadcast flag
    for (auto const& [s, c] : _clients)
      if (s == sock ||
          std::to_underlying(c.bc_flags & BroadcastFlags::SystemStatus)) {
        this->transmit(s, reply);
        logf('S', s, "LAN_SYSTEMSTATE_DATACHANGED", reply);
      }
  }

  /// \todo document
  ///
  /// This message is sent to the clients by the Z21 from FW version 1.29 on as
  /// a response to the command LAN_RAILCOM_GETDATA. However, it is also sent to
  /// clients unsolicitedly, if
  /// - the corresponding RailCom data have actually changed
  /// - and the associated client has activated the corresponding broadcast and
  ///   the associated client has subscribed to the locomotive address with
  ///   LAN_X_GET_LOCO_INFO.
  /// - or the associated client has subscribed to broadcast 0x00040000 (i.e.
  ///   RailCom data of all locomotives, for PC control SW only).
  void lanRailComDataChanged(uint16_t loco_addr, Socket const& sock = {})
    requires(std::derived_from<Base, intf::RailCom>)
  {
    auto const railcom_data{this->railComData(loco_addr)};

    std::array<uint8_t, 0x11uz> const reply{
      0x11u,                                                     // Length
      0x00u,                                                     //
      std::to_underlying(Header::LAN_RAILCOM_DATACHANGED),       // Header
      0x00u,                                                     //
      static_cast<uint8_t>(railcom_data.loco_address >> 0u),     //
      static_cast<uint8_t>(railcom_data.loco_address >> 8u),     //
      static_cast<uint8_t>(railcom_data.receive_counter >> 0u),  //
      static_cast<uint8_t>(railcom_data.receive_counter >> 8u),  //
      static_cast<uint8_t>(railcom_data.receive_counter >> 16u), //
      static_cast<uint8_t>(railcom_data.receive_counter >> 24u), //
      static_cast<uint8_t>(railcom_data.error_counter >> 0u),    //
      static_cast<uint8_t>(railcom_data.error_counter >> 8u),    //
      0x00u,                                                     // Reserved
      railcom_data.options,                                      //
      railcom_data.speed,                                        //
      railcom_data.qos,                                          //
      0x00u                                                      // Reserved
    };

    //
    if (sock) {
      this->transmit(sock, reply);
      logf('S', sock, "LAN_RAILCOM_DATACHANGED", reply);
    }
    //
    else
      for (auto const& [s, c] : _clients)
        if ((std::to_underlying(c.bc_flags &
                                BroadcastFlags::RailComSubscribed) &&
             std::ranges::contains(c.sub_loco_addrs, loco_addr)) ||
            std::to_underlying(c.bc_flags & BroadcastFlags::RailCom)) {
          this->transmit(s, reply);
          logf('S', s, "LAN_RAILCOM_DATACHANGED", reply);
        }
  }

  /// \todo document
  void lanLocoNetZ21Rx(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanLocoNetZ21Tx(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanLocoNetFromLan(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanLocoNetDispatchAddr(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanLocoNetDetector(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanCanDetector(Socket const&) {
    /// \todo
  }

  /// \todo document
  void replyToLanCanDeviceGetDescription(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanCanBoosterSystemStateDataChanged(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanFastClockData(Socket const&) {
    /// \todo
  }

  /// \todo document
  void replyTolanFastClockSettingsGet(Socket const&) {
    /// \todo
  }

  /// \todo document
  void replyToLanBoosterGetDescription(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanBoosterSystemStateDataChanged(Socket const&) {
    /// \todo
  }

  /// \todo document
  void replyToLanDecoderGetDescription(Socket const&) {
    /// \todo
  }

  /// \todo document
  void lanDecoderSystemStateDataChanged(Socket const&) {
    /// \todo
  }

  /// \todo document
  void replyToLanZLinkGetHwInfo(Socket const&) {
    /// \todo
  }

private:
  /// \todo document
  Clients::iterator execute(Clients::iterator it) {
    auto& [sock, client]{*it};

    while (!empty(client.datasets)) {
      switch (auto const& [header, chunk]{client.datasets.front()}; header) {
        case Header::LAN_GET_SERIAL_NUMBER:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_GET_SERIAL_NUMBER", chunk);
            lanGetSerialNumber(sock);
          }
          break;

        case Header::LAN_GET_COMMON_SETTINGS:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_GET_COMMON_SETTINGS", chunk);
            if constexpr (std::derived_from<Base, intf::Settings>)
              lanGetCommonSettings(sock);
          }
          break;

        case Header::LAN_SET_COMMON_SETTINGS:
          if (size(chunk) == 0x0Euz - 4uz) {
            logf('C', sock, "LAN_SET_COMMON_SETTINGS", chunk);
            if constexpr (std::derived_from<Base, intf::Settings>)
              lanSetCommonSettings(
                sock,
                CommonSettings{
                  .enable_railcom = static_cast<bool>(chunk[0uz]),
                  .enable_bit_modify_on_long_address =
                    static_cast<bool>(chunk[1uz]),
                  .key_stop_mode =
                    static_cast<CommonSettings::KeyStopMode>(chunk[2uz]),
                  .programming_type =
                    static_cast<CommonSettings::ProgrammingType>(chunk[3uz]),
                  .enable_loconet_current_source =
                    static_cast<bool>(chunk[4uz]),
                  .loconet_fast_clock_rate = chunk[5uz],
                  .loconet_mode = chunk[6uz],
                  .ext_settings =
                    static_cast<CommonSettings::ExtFlags>(chunk[7uz]),
                  .purging_time = chunk[8uz],
                  .bus_settings = chunk[9uz]});
          }
          break;

        case Header::LAN_GET_MMDCC_SETTINGS:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_GET_MMDCC_SETTINGS", chunk);
            if constexpr (std::derived_from<Base, intf::Settings>)
              lanGetMmDccSettings(sock);
          }
          break;

        case Header::LAN_SET_MMDCC_SETTINGS:
          if (size(chunk) == 0x14uz - 4uz) {
            logf('C', sock, "LAN_SET_MMDCC_SETTINGS", chunk);
            if constexpr (std::derived_from<Base, intf::Settings>)
              lanSetMmDccSettings(
                sock,
                MmDccSettings{
                  .startup_reset_package_count = chunk[0uz],
                  .continue_reset_packet_count = chunk[1uz],
                  .program_package_count = chunk[2uz],
                  .bit_verify_to_one = static_cast<bool>(chunk[3uz]),
                  .programming_ack_current = chunk[10uz],
                  .flags = static_cast<MmDccSettings::Flags>(chunk[11uz]),
                  .output_voltage = static_cast<uint16_t>(chunk[13uz] << 8u |
                                                          chunk[12uz] << 0u),
                  .programming_voltage = static_cast<uint16_t>(
                    chunk[15uz] << 8u | chunk[14uz] << 0u)});
          }
          break;

        case Header::LAN_GET_CODE:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_GET_CODE", chunk);
            lanGetCode(sock);
          }
          break;

        case Header::LAN_GET_HWINFO:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_GET_HWINFO", chunk);
            lanGetHwInfo(sock);
          }
          break;

        // LAN_LOGOFF erases the client, it is crucial to return here!
        case Header::LAN_LOGOFF:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_LOGOFF", chunk);
            return lanLogoff(sock, it);
          }
          break;

        case Header::LAN_X:
          if (size(chunk) >= 0x06uz - 4uz) {
#if Z21_XBUS_ENABLE_CHECKSUM
            if (exor(chunk)) {
              logf('C', sock, "Wrong checksum", chunk);
              break;
            }
#endif
            switch (static_cast<XHeader>(chunk[0uz])) {
              case XHeader::LAN_X_21:
                switch (static_cast<DB0>(chunk[1uz])) {
                  case DB0::LAN_X_GET_VERSION:
                    if (size(chunk) == 0x07uz - 4uz) {
                      logf('C', sock, "LAN_X_GET_VERSION", chunk);
                      lanXGetVersion(sock);
                    }
                    break;
                  case DB0::LAN_X_GET_STATUS:
                    if (size(chunk) == 0x07uz - 4uz) {
                      logf('C', sock, "LAN_X_GET_STATUS", chunk);
                      lanXGetStatus(sock);
                    }
                    break;
                  case DB0::LAN_X_SET_TRACK_POWER_OFF:
                    if (size(chunk) == 0x07uz - 4uz) {
                      logf('C', sock, "LAN_X_SET_TRACK_POWER_OFF", chunk);
                      lanXSetTrackPowerOff(sock);
                    }
                    break;
                  case DB0::LAN_X_SET_TRACK_POWER_ON:
                    if (size(chunk) == 0x07uz - 4uz) {
                      logf('C', sock, "LAN_X_SET_TRACK_POWER_ON", chunk);
                      lanXSetTrackPowerOn(sock);
                    }
                    break;
                  default:
                    logf('C',
                         sock,
                         "LAN_X_UNKNOWN_COMMAND",
                         chunk,
                         "%02X",
                         header);
                    lanXUnknownCommand(sock);
                    break;
                }
                break;

              case XHeader::LAN_X_22:
                switch (static_cast<DB0>(chunk[1uz])) {
                  case DB0::LAN_X_DCC_READ_REGISTER:
                    if (size(chunk) == 0x08uz - 4uz) {
                      logf('C', sock, "LAN_X_DCC_READ_REGISTER", chunk);
                      if constexpr (std::derived_from<Base, intf::Programming>)
                        lanXDccReadRegister(sock);
                    }
                    break;
                  case DB0::LAN_X_CV_READ_ALT:
                    if (size(chunk) == 0x08uz - 4uz) {
                      logf('C', sock, "LAN_X_CV_READ_ALT", chunk);
                      if constexpr (std::derived_from<Base, intf::Programming>)
                        lanXCvRead(sock, static_cast<uint8_t>(chunk[2uz] - 1u));
                    }
                    break;
                  default:
                    logf('C',
                         sock,
                         "LAN_X_UNKNOWN_COMMAND",
                         chunk,
                         "%02X",
                         header);
                    lanXUnknownCommand(sock);
                    break;
                }
                break;

              case XHeader::LAN_X_23:
                switch (static_cast<DB0>(chunk[1uz])) {
                  case DB0::LAN_X_DCC_WRITE_REGISTER:
                    if (size(chunk) == 0x09uz - 4uz) {
                      logf('C', sock, "LAN_X_DCC_WRITE_REGISTER", chunk);
                      if constexpr (std::derived_from<Base, intf::Programming>)
                        lanXDccWriteRegister(sock);
                    }
                    break;
                  case DB0::LAN_X_CV_READ:
                    if (size(chunk) == 0x09uz - 4uz) {
                      logf('C', sock, "LAN_X_CV_READ", chunk);
                      if constexpr (std::derived_from<Base, intf::Programming>)
                        lanXCvRead(sock,
                                   big_endian_data2cv_address(data(chunk) + 2));
                    }
                    break;
                  case DB0::LAN_X_CV_READ_ALT:
                    if (size(chunk) == 0x09uz - 4uz) {
                      logf('C', sock, "LAN_X_CV_READ_ALT", chunk);
                      if constexpr (std::derived_from<Base, intf::Programming>)
                        lanXCvRead(sock,
                                   big_endian_data2cv_address(data(chunk) + 2));
                    }
                    break;
                  case DB0::LAN_X_CV_WRITE_ALT:
                    if (size(chunk) == 0x09uz - 4uz) {
                      logf('C', sock, "LAN_X_CV_WRITE_ALT", chunk);
                      if constexpr (std::derived_from<Base, intf::Programming>)
                        lanXCvWrite(sock,
                                    static_cast<uint8_t>(chunk[2uz] - 1u),
                                    chunk[3uz]);
                    }
                    break;
                  default:
                    logf('C',
                         sock,
                         "LAN_X_UNKNOWN_COMMAND",
                         chunk,
                         "%02X",
                         header);
                    lanXUnknownCommand(sock);
                    break;
                }
                break;

              case XHeader::LAN_X_24:
                switch (static_cast<DB0>(chunk[1uz])) {
                  case DB0::LAN_X_CV_WRITE:
                    if (size(chunk) == 0x0Auz - 4uz) {
                      logf('C', sock, "LAN_X_CV_WRITE", chunk);
                      if constexpr (std::derived_from<Base, intf::Programming>)
                        lanXCvWrite(sock,
                                    big_endian_data2cv_address(data(chunk) + 2),
                                    chunk[4uz]);
                    }
                    break;
                  case DB0::LAN_X_CV_WRITE_ALT:
                    if (size(chunk) == 0x0Auz - 4uz) {
                      logf('C', sock, "LAN_X_CV_WRITE_ALT", chunk);
                      if constexpr (std::derived_from<Base, intf::Programming>)
                        lanXCvWrite(sock,
                                    big_endian_data2cv_address(data(chunk) + 2),
                                    chunk[4uz]);
                    }
                    break;
                  case DB0::LAN_X_MM_WRITE_BYTE:
                    if (size(chunk) == 0x0Auz - 4uz) {
                      logf('C', sock, "LAN_X_MM_WRITE_BYTE", chunk);
                      if constexpr (std::derived_from<Base, intf::Programming>)
                        lanXMmWriteByte(sock);
                    }
                    break;
                  default:
                    logf('C',
                         sock,
                         "LAN_X_UNKNOWN_COMMAND",
                         chunk,
                         "%02X",
                         header);
                    lanXUnknownCommand(sock);
                    break;
                }
                break;

              case XHeader::LAN_X_GET_TURNOUT_INFO:
                if (size(chunk) == 0x08uz - 4uz) {
                  logf('C', sock, "LAN_X_GET_TURNOUT_INFO", chunk);
                  if constexpr (std::derived_from<Base, intf::Switching>)
                    lanXGetTurnoutInfo(
                      sock, big_endian_data2accessory_address(data(chunk) + 1));
                }
                break;

              case XHeader::LAN_X_GET_EXT_ACCESSORY_INFO:
                if (size(chunk) == 0x09uz - 4uz) {
                  logf('C', sock, "LAN_X_GET_EXT_ACCESSORY_INFO", chunk);
                  if constexpr (std::derived_from<Base, intf::Switching>)
                    lanXGetExtAccessoryInfo(
                      sock, big_endian_data2accessory_address(data(chunk) + 1));
                }
                break;

              case XHeader::LAN_X_SET_TURNOUT:
                if (size(chunk) == 0x09uz - 4uz) {
                  logf('C', sock, "LAN_X_SET_TURNOUT", chunk);
                  if constexpr (std::derived_from<Base, intf::Switching>)
                    lanXSetTurnout(
                      sock,
                      big_endian_data2accessory_address(data(chunk) + 1),
                      chunk[3uz] & 0b0000'0001u,  // P
                      chunk[3uz] & 0b0000'1000u,  // A
                      chunk[3uz] & 0b0010'0000u); // Q
                }
                break;

              case XHeader::LAN_X_SET_EXT_ACCESSORY:
                if (size(chunk) == 0x0Auz - 4uz) {
                  logf('C', sock, "LAN_X_SET_EXT_ACCESSORY", chunk);
                  if constexpr (std::derived_from<Base, intf::Switching>)
                    lanXSetExtAccessory(
                      sock,
                      big_endian_data2accessory_address(data(chunk) + 1),
                      chunk[3uz]);
                }
                break;

              case XHeader::LAN_X_SET_STOP:
                if (size(chunk) == 0x06uz - 4uz) {
                  logf('C', sock, "LAN_X_SET_STOP", chunk);
                  lanXSetStop(sock);
                }
                break;

              case XHeader::LAN_X_SET_LOCO_E_STOP:
                if (size(chunk) == 0x08uz - 4uz) {
                  logf('C', sock, "LAN_X_SET_LOCO_E_STOP", chunk);
                  if constexpr (std::derived_from<Base, intf::Driving>)
                    lanXSetLocoEStop(
                      sock, big_endian_data2loco_address(data(chunk) + 2));
                }
                break;

              case XHeader::LAN_X_E3:
                switch (static_cast<DB0>(chunk[1uz])) {
                  case DB0::LAN_X_PURGE_LOCO:
                    if (size(chunk) == 0x09uz - 4uz) {
                      logf('C', sock, "LAN_X_PURGE_LOCO", chunk);
                      if constexpr (std::derived_from<Base, intf::Driving>)
                        lanXPurgeLoco(
                          sock, big_endian_data2loco_address(data(chunk) + 2));
                    }
                    break;
                  case DB0::LAN_X_GET_LOCO_INFO:
                    if (size(chunk) == 0x09uz - 4uz) {
                      logf('C', sock, "LAN_X_GET_LOCO_INFO", chunk);
                      if constexpr (std::derived_from<Base, intf::Driving>)
                        lanXGetLocoInfo(
                          sock, big_endian_data2loco_address(data(chunk) + 2));
                    }
                    break;
                  default:
                    logf('C',
                         sock,
                         "LAN_X_UNKNOWN_COMMAND",
                         chunk,
                         "%02X",
                         header);
                    lanXUnknownCommand(sock);
                    break;
                }
                break;

              case XHeader::LAN_X_E4:
                switch (static_cast<DB0>(chunk[1uz])) {
                  case DB0::LAN_X_SET_LOCO_DRIVE_14:
                    if (size(chunk) == 0x0Auz - 4uz) {
                      logf('C', sock, "LAN_X_SET_LOCO_DRIVE", chunk);
                      if constexpr (std::derived_from<Base, intf::Driving>)
                        lanXSetLocoDrive(
                          sock,
                          big_endian_data2loco_address(data(chunk) + 2),
                          LocoInfo::DCC14,
                          chunk[4uz]);
                    }
                    break;
                  case DB0::LAN_X_SET_LOCO_DRIVE_28:
                    if (size(chunk) == 0x0Auz - 4uz) {
                      logf('C', sock, "LAN_X_SET_LOCO_DRIVE", chunk);
                      if constexpr (std::derived_from<Base, intf::Driving>)
                        lanXSetLocoDrive(
                          sock,
                          big_endian_data2loco_address(data(chunk) + 2),
                          LocoInfo::DCC28,
                          chunk[4uz]);
                    }
                    break;
                  case DB0::LAN_X_SET_LOCO_DRIVE_128:
                    if (size(chunk) == 0x0Auz - 4uz) {
                      logf('C', sock, "LAN_X_SET_LOCO_DRIVE", chunk);
                      if constexpr (std::derived_from<Base, intf::Driving>)
                        lanXSetLocoDrive(
                          sock,
                          big_endian_data2loco_address(data(chunk) + 2),
                          LocoInfo::DCC128,
                          chunk[4uz]);
                    }
                    break;

                  case DB0::LAN_X_SET_LOCO_FUNCTION:
                    if (size(chunk) == 0x0Auz - 4uz) {
                      logf('C', sock, "LAN_X_SET_LOCO_FUNCTION", chunk);
                      if constexpr (std::derived_from<Base, intf::Driving>)
                        lanXSetLocoFunction(
                          sock,
                          big_endian_data2loco_address(data(chunk) + 2),
                          chunk[4uz] >> 6uz,
                          chunk[4uz] & 0x3Fu);
                    }
                    break;

                  case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_1: [[fallthrough]];
                  case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_2: [[fallthrough]];
                  case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_3: [[fallthrough]];
                  case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_4: [[fallthrough]];
                  case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_5: [[fallthrough]];
                  case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_6: [[fallthrough]];
                  case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_7: [[fallthrough]];
                  case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_8: [[fallthrough]];
                  case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_9: [[fallthrough]];
                  case DB0::LAN_X_SET_LOCO_FUNCTION_GROUP_10:
                    if (size(chunk) == 0x0Auz - 4uz) {
                      logf('C', sock, "LAN_X_SET_LOCO_FUNCTION_GROUP", chunk);
                      if constexpr (std::derived_from<Base, intf::Driving>)
                        lanXSetLocoFunctionGroup(
                          sock,
                          big_endian_data2loco_address(data(chunk) + 2),
                          chunk[1uz],
                          chunk[4uz]);
                    }
                    break;

                  case DB0::LAN_X_SET_LOCO_BINARY_STATE:
                    if (size(chunk) == 0x0Auz - 4uz) {
                      logf('C', sock, "LAN_X_SET_LOCO_BINARY_STATE", chunk);
                      if constexpr (std::derived_from<Base, intf::Driving>)
                        lanXSetLocoBinaryState(sock);
                    }
                    break;

                  default:
                    logf('C',
                         sock,
                         "LAN_X_UNKNOWN_COMMAND",
                         chunk,
                         "%02X",
                         header);
                    lanXUnknownCommand(sock);
                    break;
                }
                break;

              case XHeader::LAN_X_E6:
                switch (static_cast<DB0>(chunk[1uz])) {
                  case DB0::LAN_X_CV_POM:
                    switch (chunk[4uz] & 0xFCu) {
                      case 0xECu:
                        if (size(chunk) == 0x0Cuz - 4uz) {
                          logf('C', sock, "LAN_X_CV_POM_WRITE_BYTE", chunk);
                          if constexpr (std::derived_from<Base,
                                                          intf::Programming>)
                            lanXCvPomWriteByte(
                              sock,
                              big_endian_data2loco_address(data(chunk) + 2),
                              big_endian_data2cv_address(data(chunk) + 4),
                              chunk[6uz]);
                        }
                        break;
                      case 0xE8u:
                        if (size(chunk) == 0x0Cuz - 4uz) {
                          logf('C', sock, "LAN_X_CV_POM_WRITE_BIT", chunk);
                          if constexpr (std::derived_from<Base,
                                                          intf::Programming>)
                            lanXCvPomWriteBit(sock);
                        }
                        break;
                      case 0xE4u:
                        if (size(chunk) == 0x0Cuz - 4uz) {
                          logf('C', sock, "LAN_X_CV_POM_READ_BYTE", chunk);
                          if constexpr (std::derived_from<Base,
                                                          intf::Programming>)
                            lanXCvPomReadByte(
                              sock,
                              big_endian_data2loco_address(data(chunk) + 2),
                              big_endian_data2cv_address(data(chunk) + 4));
                        }
                        break;
                    }
                    break;
                  case DB0::LAN_X_CV_POM_ACCESSORY:
                    switch (chunk[4uz] & 0xFCu) {
                      case 0xECu:
                        if (size(chunk) == 0x0Cuz - 4uz) {
                          logf('C',
                               sock,
                               "LAN_X_CV_POM_ACCESSORY_WRITE_BYTE",
                               chunk);
                          if constexpr (std::derived_from<Base,
                                                          intf::Programming>)
                            lanXCvPomAccessoryWriteByte(
                              sock,
                              stupid_data2accessory_address(data(chunk) + 2),
                              big_endian_data2cv_address(data(chunk) + 4),
                              chunk[6uz],
                              chunk[3uz] & 0x08u);
                        }
                        break;
                      case 0xE8u:
                        if (size(chunk) == 0x0Cuz - 4uz) {
                          logf('C',
                               sock,
                               "LAN_X_CV_POM_ACCESSORY_WRITE_BIT",
                               chunk);
                          if constexpr (std::derived_from<Base,
                                                          intf::Programming>)
                            lanXCvPomAccessoryWriteBit(sock);
                        }
                        break;
                      case 0xE4u:
                        if (size(chunk) == 0x0Cuz - 4uz) {
                          logf('C',
                               sock,
                               "LAN_X_CV_POM_ACCESSORY_READ_BYTE",
                               chunk);
                          if constexpr (std::derived_from<Base,
                                                          intf::Programming>)
                            lanXCvPomAccessoryReadByte(
                              sock,
                              stupid_data2accessory_address(data(chunk) + 2),
                              big_endian_data2cv_address(data(chunk) + 4),
                              chunk[3uz] & 0x08u);
                        }
                        break;
                    }
                    break;
                  default:
                    logf('C',
                         sock,
                         "LAN_X_UNKNOWN_COMMAND",
                         chunk,
                         "%02X",
                         header);
                    lanXUnknownCommand(sock);
                    break;
                }
                break;

              case XHeader::LAN_X_GET_FIRMWARE_VERSION:
                switch (static_cast<DB0>(chunk[1uz])) {
                  case DB0::LAN_X_GET_FIRMWARE_VERSION:
                    if (size(chunk) == 0x07uz - 4uz) {
                      logf('C', sock, "LAN_X_GET_FIRMWARE_VERSION", chunk);
                      lanXGetFirmwareVersion(sock);
                    }
                    break;
                  default:
                    logf('C',
                         sock,
                         "LAN_X_UNKNOWN_COMMAND",
                         chunk,
                         "%02X",
                         header);
                    lanXUnknownCommand(sock);
                    break;
                }
                break;

              default:
                logf('C', sock, "LAN_X_UNKNOWN_COMMAND", chunk, "%02X", header);
                lanXUnknownCommand(sock);
                break;
            }
          }
          break;

        case Header::LAN_SET_BROADCASTFLAGS:
          if (size(chunk) == 0x08uz - 4uz) {
            logf('C', sock, "LAN_SET_BROADCASTFLAGS", chunk);
            lanSetBroadcastFlags(sock,
                                 static_cast<BroadcastFlags>(
                                   little_endian_data2uint32(data(chunk))));
          }
          break;

        case Header::LAN_GET_BROADCASTFLAGS:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_GET_BROADCASTFLAGS", chunk);
            lanGetBroadcastFlags(sock);
          }
          break;

        case Header::LAN_GET_LOCOMODE:
          if (size(chunk) == 0x06uz - 4uz) {
            logf('C', sock, "LAN_GET_LOCOMODE", chunk);
            if constexpr (std::derived_from<Base, intf::Driving>)
              lanGetLocoMode(sock, big_endian_data2loco_address(data(chunk)));
          }
          break;

        case Header::LAN_SET_LOCOMODE:
          if (size(chunk) == 0x07uz - 4uz) {
            logf('C', sock, "LAN_SET_LOCOMODE", chunk);
            if constexpr (std::derived_from<Base, intf::Driving>)
              lanSetLocoMode(sock,
                             big_endian_data2loco_address(data(chunk)),
                             static_cast<LocoInfo::Mode>(chunk[2uz]));
          }
          break;

        case Header::LAN_GET_TURNOUTMODE:
          if (size(chunk) == 0x06uz - 4uz) {
            logf('C', sock, "LAN_GET_TURNOUTMODE", chunk);
            if constexpr (std::derived_from<Base, intf::Switching>)
              lanGetTurnoutMode(sock,
                                big_endian_data2accessory_address(data(chunk)));
          }
          break;

        case Header::LAN_SET_TURNOUTMODE:
          if (size(chunk) == 0x07uz - 4uz) {
            logf('C', sock, "LAN_SET_TURNOUTMODE", chunk);
            if constexpr (std::derived_from<Base, intf::Switching>)
              lanSetTurnoutMode(sock,
                                big_endian_data2accessory_address(data(chunk)),
                                static_cast<TurnoutInfo::Mode>(chunk[2uz]));
          }
          break;

        case Header::LAN_RMBUS_GETDATA:
          if (size(chunk) == 0x05uz - 4uz) {
            logf('C', sock, "LAN_RMBUS_GETDATA", chunk);
            if constexpr (std::derived_from<Base, intf::RBus>)
              lanRmBusGetData(sock);
          }
          break;

        case Header::LAN_RMBUS_PROGRAMMODULE:
          if (size(chunk) == 0x05uz - 4uz) {
            logf('C', sock, "LAN_RMBUS_PROGRAMMODULE", chunk);
            if constexpr (std::derived_from<Base, intf::RBus>)
              lanRmBusProgramModule(sock);
          }
          break;

        case Header::LAN_SYSTEMSTATE_GETDATA:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_SYSTEMSTATE_GETDATA", chunk);
            lanSystemStateGetData(sock);
          }
          break;

        case Header::LAN_RAILCOM_GETDATA:
          if (size(chunk) == 0x07uz - 4uz) {
            logf('C', sock, "LAN_RAILCOM_GETDATA", chunk);
            if constexpr (std::derived_from<Base, intf::RailCom>)
              lanRailComGetData(
                sock,
                chunk[0uz],
                little_endian_data2loco_address(data(chunk) + 1));
          }
          break;

        case Header::LAN_LOCONET_FROM_LAN:
          if (size(chunk) > 0x04uz - 4uz) {
            logf('C', sock, "LAN_LOCONET_FROM_LAN", chunk);
            if constexpr (std::derived_from<Base, intf::LocoNet>)
              lanLocoNetFromLan(sock);
          }
          break;

        case Header::LAN_LOCONET_DISPATCH_ADDR:
          if (size(chunk) == 0x06uz - 4uz) {
            logf('C', sock, "LAN_LOCONET_DISPATCH_ADDR", chunk);
            if constexpr (std::derived_from<Base, intf::LocoNet>)
              lanLocoNetDispatchAddr(sock);
          }
          break;

        case Header::LAN_LOCONET_DETECTOR:
          if (size(chunk) == 0x07uz - 4uz) {
            logf('C', sock, "LAN_LOCONET_DETECTOR", chunk);
            if constexpr (std::derived_from<Base, intf::LocoNet>)
              lanLocoNetDetector(sock);
          }
          break;

        case Header::LAN_CAN_DETECTOR:
          if (size(chunk) == 0x07uz - 4uz) {
            logf('C', sock, "LAN_CAN_DETECTOR", chunk);
            if constexpr (std::derived_from<Base, intf::Can>)
              lanCanDetector(sock);
          }
          break;

        case Header::LAN_CAN_DEVICE_GET_DESCRIPTION:
          if (size(chunk) == 0x06uz - 4uz) {
            logf('C', sock, "LAN_CAN_DEVICE_GET_DESCRIPTION", chunk);
            if constexpr (std::derived_from<Base, intf::Can>)
              lanCanDeviceGetDescription(sock);
          }
          break;

        case Header::LAN_CAN_DEVICE_SET_DESCRIPTION:
          if (size(chunk) == 0x16uz - 4uz) {
            logf('C', sock, "LAN_CAN_DEVICE_SET_DESCRIPTION", chunk);
            if constexpr (std::derived_from<Base, intf::Can>)
              lanCanDeviceSetDescription(sock);
          }
          break;

        case Header::LAN_CAN_BOOSTER_SET_TRACKPOWER:
          if (size(chunk) == 0x07uz - 4uz) {
            logf('C', sock, "LAN_CAN_BOOSTER_SET_TRACKPOWER", chunk);
            if constexpr (std::derived_from<Base, intf::Can>)
              lanCanBoosterSetTrackPower(sock);
          }
          break;

        case Header::LAN_FAST_CLOCK_CONTROL:
          logf('C', sock, "LAN_FAST_CLOCK_CONTROL", chunk);
          lanFastClockControl(sock);
          break;

        case Header::LAN_FAST_CLOCK_SETTINGS_GET:
          if (size(chunk) == 0x05uz - 4uz) {
            logf('C', sock, "LAN_FAST_CLOCK_SETTINGS_GET", chunk);
            lanFastClockSettingsGet(sock);
          }
          break;

        case Header::LAN_FAST_CLOCK_SETTINGS_SET:
          logf('C', sock, "LAN_FAST_CLOCK_SETTINGS_SET", chunk);
          lanFastClockSettingsSet(sock);
          break;

        case Header::LAN_BOOSTER_SET_POWER:
          if (size(chunk) == 0x06uz - 4uz) {
            logf('C', sock, "LAN_BOOSTER_SET_POWER", chunk);
            lanBoosterSetPower(sock);
          }
          break;

        case Header::LAN_BOOSTER_GET_DESCRIPTION:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_BOOSTER_GET_DESCRIPTION", chunk);
            lanBoosterGetDescription(sock);
          }
          break;

        case Header::LAN_BOOSTER_SET_DESCRIPTION:
          if (size(chunk) == 0x24uz - 4uz) {
            logf('C', sock, "LAN_BOOSTER_SET_DESCRIPTION", chunk);
            lanBoosterSetDescription(sock);
          }
          break;

        case Header::LAN_BOOSTER_SYSTEMSTATE_GETDATA:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_BOOSTER_SYSTEMSTATE_GETDATA", chunk);
            lanBoosterSystemStateGetData(sock);
          }
          break;

        case Header::LAN_DECODER_GET_DESCRIPTION:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_DECODER_GET_DESCRIPTION", chunk);
            lanDecoderGetDescription(sock);
          }
          break;

        case Header::LAN_DECODER_SET_DESCRIPTION:
          if (size(chunk) == 0x24uz - 4uz) {
            logf('C', sock, "LAN_DECODER_SET_DESCRIPTION", chunk);
            lanDecoderSetDescription(sock);
          }
          break;

        case Header::LAN_DECODER_SYSTEMSTATE_GETDATA:
          if (size(chunk) == 0x04uz - 4uz) {
            logf('C', sock, "LAN_DECODER_SYSTEMSTATE_GETDATA", chunk);
            lanDecoderSystemStateGetData(sock);
          }
          break;

        case Header::LAN_ZLINK_GET_HWINFO:
          if (size(chunk) == 0x05uz - 4uz) {
            logf('C', sock, "LAN_ZLINK_GET_HWINFO", chunk);
            if constexpr (std::derived_from<Base, intf::ZLink>)
              lanZLinkGetHwInfo(sock);
          }
          break;

        default:
          logf('C', sock, "LAN_X_UNKNOWN_COMMAND", chunk, "%02X", header);
          lanXUnknownCommand(sock);
          break;
      }

      client.datasets.pop_front();
    }

    return ++it;
  }

  /// \todo document
  void logf(auto&&...) requires(!std::derived_from<Base, intf::Logging>)
  {}

  /// \todo document
  void logf(char c,
            Socket const& sock,
            char const* str,
            std::span<uint8_t const> chunk = {},
            auto&&... ts) requires(std::derived_from<Base, intf::Logging>)
  {
    static constexpr auto max_ip4_strlen{16uz};
    // static constexpr auto max_ip6_strlen{46uz};
    static constexpr auto len_post_prefix{sizeof(c) + sizeof(' ')};
    static constexpr auto len_post_ip{len_post_prefix + max_ip4_strlen +
                                      sizeof(' ')};
    static constexpr auto len_post_cmd{len_post_ip + 37uz + sizeof(' ')};

    // Single character identifying 'C'lient or 'S'erver message
    std::array<char, 256uz> buffer{c, ' '};

    // IP
    sockaddr_in* sin{std::bit_cast<sockaddr_in*>(&sock.addr)};
    inet_ntop(sock.addr.ss_family,
              &sin->sin_addr.s_addr,
              data(buffer) + len_post_prefix,
              size(buffer) - len_post_prefix);
    size_t len{strlen(data(buffer))};
    while (len < len_post_ip) buffer[len++] = ' ';

    // Command
    len += static_cast<size_t>(
      snprintf(data(buffer) + len, size(buffer) - len, "%s ", str));
    while (len < len_post_cmd) buffer[len++] = ' ';

    // Data
    for (auto const byte : chunk)
      len += static_cast<size_t>(
        snprintf(data(buffer) + len, size(buffer) - len, "%02X ", byte));

    // Additional arguments
    if constexpr (sizeof...(ts) > 0uz)
      snprintf(data(buffer) + len,
               size(buffer) - len,
               std::forward<decltype(ts)>(ts)...);

    this->log(data(buffer));
  }

  Clients _clients;
  ztl::inplace_deque<Socket const*, Z21_SERVER_MAX_LOCO_ADDRESSES_PER_CLIENT>
    _cv_request_deque{};
  HwType _hw_type;
};

} // namespace z21::server

#undef Z21_REQUIRES
#undef Z21_REQUIRE_BODY
