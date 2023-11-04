#pragma once

#include <sys/socket.h>
#include <span>
#include <ztl/inplace_deque.hpp>
#include <ztl/inplace_vector.hpp>

#define Z21_MAX_PACKET_SIZE 32uz - 1uz
#define Z21_MAX_DATASET_SIZE 63uz

namespace z21 {

using Dataset = ztl::inplace_vector<uint8_t, Z21_MAX_DATASET_SIZE>;

struct UdpDataset {
  int sock{};
  sockaddr addr{};
  Dataset dataset{};
};

class ServerBase {
public:
  void
  receive(int sock, sockaddr const& addr, std::span<uint8_t const> udp_payload);
  bool execute();

private:
  ztl::inplace_deque<UdpDataset, Z21_MAX_PACKET_SIZE> _deque;
};

}  // namespace z21