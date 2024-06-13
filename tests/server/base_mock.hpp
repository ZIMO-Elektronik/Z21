#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <z21/z21.hpp>

using ServerBase = z21::server::Base<z21::server::intf::System>;

struct BaseMock : ServerBase {
  MOCK_METHOD(void,
              transmit,
              (z21::Socket const&, std::span<uint8_t const> payload),
              (final));
  MOCK_METHOD(void, trackPower, (bool), (final));
  MOCK_METHOD(void, stop, (), (final));
  MOCK_METHOD(void, logoff, (z21::Socket const&), (final));
};