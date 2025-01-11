#include "utility.hpp"

using namespace z21;

Socket fd_ip2socket(int sock_fd, std::string ip) {
  sockaddr_in dest_addr_ip4;
  inet_pton(AF_INET, ip.c_str(), &(dest_addr_ip4.sin_addr));
  return {
    sock_fd, std::bit_cast<sockaddr*>(&dest_addr_ip4), sizeof(dest_addr_ip4)};
}

std::vector<uint8_t> make_lan_set_broadcastflags_dataset(BroadcastFlags flags) {
  return make_lan_set_broadcastflags_dataset(std::to_underlying(flags));
}

std::vector<uint8_t> make_lan_set_broadcastflags_dataset(uint32_t flags) {
  return {0x08u,
          0x00u,
          std::to_underlying(Header::LAN_SET_BROADCASTFLAGS),
          0x00u,
          static_cast<uint8_t>(flags >> 0u),   //
          static_cast<uint8_t>(flags >> 8u),   //
          static_cast<uint8_t>(flags >> 16u),  //
          static_cast<uint8_t>(flags >> 24u)}; //
}

std::vector<uint8_t> make_lan_logoff_dataset() {
  return {0x04u, 0x00u, std::to_underlying(Header::LAN_LOGOFF), 0x00u};
}

std::vector<uint8_t> make_lan_x_set_track_power_off_dataset() {
  return {0x07u,
          0x00u,
          std::to_underlying(Header::LAN_X_SET_TRACK_POWER_OFF),
          0x00u,
          std::to_underlying(XHeader::LAN_X_SET_TRACK_POWER_OFF),
          std::to_underlying(DB0::LAN_X_SET_TRACK_POWER_OFF),
          0xA1u};
}

std::vector<uint8_t> make_lan_x_set_track_power_on_dataset() {
  return {0x07u,
          0x00u,
          std::to_underlying(Header::LAN_X_SET_TRACK_POWER_ON),
          0x00u,
          std::to_underlying(XHeader::LAN_X_SET_TRACK_POWER_ON),
          std::to_underlying(DB0::LAN_X_SET_TRACK_POWER_ON),
          0xA0u};
}