#pragma once

#include <z21/z21.hpp>

using ServerBase =
  z21::server::Base<z21::server::intf::System, z21::server::intf::Logging>;

// Bare minimum server implementing only the system and log API
class Server final : public ServerBase {
private:
  // Transmit UDP dataset on socket
  void transmit(z21::Socket const& sock,
                std::span<uint8_t const> datasets) final;

  // Enable/disable track power
  bool trackPower(bool on) final;

  // Emergency stop
  bool stop() final;

  // Log received and transmitted data
  void log(char const* str) final;

  bool _track_power{};
};
