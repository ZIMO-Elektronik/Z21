#include "z21.hpp"

namespace z21 {

void ServerBase::receive(int sock,
                         sockaddr const& addr,
                         std::span<uint8_t const> udp_payload) {
  if (full(_deque)) return;

  auto first{cbegin(udp_payload)};
  auto const last{cend(udp_payload)};
  while (first < last) {
    auto& [s, a, d]{*end(_deque)};

    s = sock;

    a = addr;

    assert(last - first >= 2);
    auto const len{first[1uz] << 8u | first[0uz]};
    std::ranges::copy_n(first, len, begin(d));
    d.resize(static_cast<Dataset::size_type>(len));

    _deque.push_back();

    first += len;
  }
}

bool ServerBase::execute() {
  if (empty(_deque)) return false;
  _deque.pop_front();
  return true;
}

}  // namespace z21