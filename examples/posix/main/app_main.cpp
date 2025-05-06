#include <array>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <thread>
#include "get_socket.hpp"
#include "server.hpp"

namespace {

using namespace std::chrono_literals;

void server_task(int sock) {
  // IPv4 Internet domain socket address
  sockaddr_in dest_addr_ip4;
  socklen_t socklen{sizeof(dest_addr_ip4)};

  // Receive buffer
  std::array<uint8_t, Z21_MAX_PAYLOAD_SIZE> rx;

  // Receive UDP datasets using recvfrom and execute them in an endless loop
  for (Server server;;) {
    if (auto const len{recvfrom(sock,
                                std::bit_cast<char*>(data(rx)),
                                sizeof(rx) - 1,
                                0,
                                std::bit_cast<sockaddr*>(&dest_addr_ip4),
                                &socklen)};
        len < 0) {
      printf("recvfrom failed %s\n", strerror(errno));
      std::exit(-1);
    } else if (len > 0) {
      server.receive({sock, std::bit_cast<sockaddr*>(&dest_addr_ip4), socklen},
                     {data(rx), static_cast<size_t>(len)});
      server.execute();
    }

    std::this_thread::sleep_for(10ms);
  }
}

} // namespace

// ESP32 entry point
extern "C" void app_main() {
  if (auto const sock{get_blocking_socket()}; sock >= 0) {
    printf("socket bound, port %d\n", z21::port);
    std::thread server_thread{server_task, sock};
    server_thread.join();
  } else {
    printf("socket failed %s\n", strerror(errno));
    std::exit(-1);
  }
}

// Entry point for all other platforms
#if !defined(ESP_PLATFORM)
int main() { app_main(); }
#endif
