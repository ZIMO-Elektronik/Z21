// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

/// Socket
///
/// \file   z21/socket.hpp
/// \author Vincent Hamp
/// \date   12/11/2023

#pragma once

#include <cstring>
#include "socket.h"

namespace z21 {

/// \todo document
struct Socket {
  constexpr Socket() = default;
  constexpr Socket(int sock_fd, sockaddr* sock_addr, socklen_t sock_len)
    : fd{sock_fd}, len{sock_len} {
    memcpy(&addr, sock_addr, static_cast<size_t>(sock_len));
  }

  constexpr operator bool() const { return *this != Socket{}; }

  friend constexpr bool operator==(Socket const& lhs, Socket const& rhs) {
    return !memcmp(&lhs, &rhs, sizeof(Socket));
  }

  friend constexpr bool operator<(Socket const& lhs, Socket const& rhs) {
    return lhs.fd != rhs.fd ? lhs.fd < rhs.fd
                            : memcmp(&lhs, &rhs, sizeof(sockaddr_storage)) < 0;
  }

  int fd{};
  sockaddr_storage addr{};
  socklen_t len{};
};

} // namespace z21
