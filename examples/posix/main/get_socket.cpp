#include "get_socket.hpp"
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <z21/z21.hpp>

namespace {

int get_socket([[maybe_unused]] bool blocking) {
  // ESP only supports blocking
#if defined(ESP_PLATFORM)
  auto const sock{socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)};
  if (sock < 0) {
    printf("socket failed %s\n", strerror(errno));
    return -1;
  }

  // Linux is simple, use socket and either open it blocking or non-blocking
#elif defined(__linux__)
  auto const sock{socket(
    AF_INET, blocking ? SOCK_DGRAM : SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_IP)};
  if (sock < 0) {
    printf("socket failed %s\n", strerror(errno));
    return -1;
  }

  // macOS does not support setting SOCK_NONBLOCK at creation time
#elif defined(__APPLE__)
  auto const sock{socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)};
  if (sock < 0) {
    printf("socket failed %s\n", strerror(errno));
    return -1;
  }

  if (!blocking) {
    auto flags{fcntl(sock, F_GETFL, 0)};
    if (flags < 0) {
      printf("fcntl failed %s\n", strerror(errno));
      return -1;
    }
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {
      printf("fcntl failed %s\n", strerror(errno));
      return -1;
    }
  }

  // On Windows things are a little more involved...
#elif defined(__WIN32__)
  // Apparently there is some WSA stuff one needs to do prior to anything:
  // https://stackoverflow.com/a/4642169/5840652
  WORD version_wanted{MAKEWORD(1, 1)};
  WSADATA wsa_data;
  if (auto const err{WSAStartup(version_wanted, &wsa_data)}; err) {
    printf("WSAStartup failed %d\n", err);
    return -1;
  }

  // Only then we can open a socket
  auto const sock{static_cast<int>(socket(AF_INET, SOCK_DGRAM, IPPROTO_IP))};
  if (sock < 0) {
    printf("socket failed %s\n", strerror(errno));
    return -1;
  }

  // Also Windows does not support opening it non-blocking so use ioctlsocket
  if (!blocking) {
    u_long mode{1}; // 1 to enable non-blocking socket
    if (auto const err{ioctlsocket(sock, FIONBIO, &mode)}) {
      printf("ioctlsocket failed %d\n", err);
      return -1;
    }
  }
#else
#  error "unsupported architecture"
#endif

  // Currently IP4 only
  sockaddr_in dest_addr_ip4;
  dest_addr_ip4.sin_addr.s_addr = htonl(INADDR_ANY);
  dest_addr_ip4.sin_family = AF_INET;
  dest_addr_ip4.sin_port = htons(z21::port);
  if (auto const err{bind(sock,
                          std::bit_cast<sockaddr*>(&dest_addr_ip4),
                          sizeof(dest_addr_ip4))}) {
    printf("bind failed %s\n", strerror(errno));
    return -1;
  }

  return sock;
}

} // namespace

int get_blocking_socket() { return get_socket(true); }

int get_non_blocking_socket() {
#if defined(ESP_PLATFORM)
  printf("socket failed %s\n", strerror(EOPNOTSUPP));
  return -1;
#endif
  return get_socket(false);
}
