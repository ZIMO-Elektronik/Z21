#include <netinet/in.h>
#include <sys/socket.h>
#include <array>
#include <bit>
#include <cassert>
#include <cstdio>
#include <thread>
#include <z21/z21.hpp>
#include "log.h"

namespace {

using namespace std::chrono_literals;

constexpr uint16_t port{21105u};

//
int init_socket() {
  //
  auto sock{socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)};
  assert(sock >= 0);
  LOGI("Socket created");

  //
  sockaddr_in6 dest_addr;
  sockaddr_in* dest_addr_ip4{std::bit_cast<sockaddr_in*>(&dest_addr)};
  dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
  dest_addr_ip4->sin_family = AF_INET;
  dest_addr_ip4->sin_port = htons(port);
  auto err{bind(sock, std::bit_cast<sockaddr*>(&dest_addr), sizeof(dest_addr))};
  assert(err >= 0);
  LOGI("Socket bound, port %d", port);

  return sock;
}

//
void server_task(int sock) {
  //
  sockaddr addr;
  socklen_t socklen{sizeof(addr)};

  std::array<uint8_t, 1472uz> rx;
  std::array<char, 128uz> addr_str;

  z21::ServerBase server_base;

  for (;;) {
    std::this_thread::sleep_for(1ms);

    LOGI("Wait for data");
    auto const len{
      recvfrom(sock, data(rx), sizeof(rx) - 1, 0, &addr, &socklen)};
    LOGI("Got %d bytes", len);

    server_base.receive(sock, addr, {data(rx), static_cast<size_t>(len)});

    while (server_base.execute())
      LOGI("Execute");
  }
}

}  // namespace

int main() {
  //
  auto sock{init_socket()};

  std::jthread server_thread{server_task, sock};

  server_thread.join();
  std::exit(0);
}