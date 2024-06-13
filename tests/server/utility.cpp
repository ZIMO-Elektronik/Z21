#include "utility.hpp"

z21::Socket fd_ip2socket(int sock_fd, std::string ip) {
  sockaddr_in dest_addr_ip4;
  inet_pton(AF_INET, ip.c_str(), &(dest_addr_ip4.sin_addr));
  return {
    sock_fd, std::bit_cast<sockaddr*>(&dest_addr_ip4), sizeof(dest_addr_ip4)};
}

std::vector<uint8_t>
make_lan_set_broadcastflags_dataset(z21::BroadcastFlags flags) {
  return make_lan_set_broadcastflags_dataset(std::to_underlying(flags));
}

std::vector<uint8_t> make_lan_set_broadcastflags_dataset(uint32_t flags) {
  return {0x08u,
          0x00u,
          std::to_underlying(z21::Header::LAN_SET_BROADCASTFLAGS),
          0x00u,
          static_cast<uint8_t>(flags >> 0u),    //
          static_cast<uint8_t>(flags >> 8u),    //
          static_cast<uint8_t>(flags >> 16u),   //
          static_cast<uint8_t>(flags >> 24u)};  //
}

std::vector<uint8_t> make_lan_logoff_dataset() {
  return {0x04u, 0x00u, std::to_underlying(z21::Header::LAN_LOGOFF), 0x00u};
}