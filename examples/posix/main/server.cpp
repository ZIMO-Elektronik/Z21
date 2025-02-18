#include "server.hpp"
#include <cerrno>
#include <cstdio>

// Implementation of transmit using sendto
void Server::transmit(z21::Socket const& sock,
                      std::span<uint8_t const> datasets) {
  auto const err{sendto(sock.fd,
                        std::bit_cast<char*>(data(datasets)),
                        size(datasets),
                        0,
                        std::bit_cast<sockaddr*>(&sock.addr),
                        sock.len)};
  if (err < 0) fprintf(stderr, "sendto failed %s", strerror(errno));
}

// Enable/disable track power
//
// In real systems this would for example set or clear a GPIO pin.
bool Server::trackPower(bool on) {
  _track_power = on;
  return true;
}

// Send emergency stop
bool Server::stop() { return true; }

// printf is sufficient
void Server::log(char const* str) { printf("%s\n", str); }