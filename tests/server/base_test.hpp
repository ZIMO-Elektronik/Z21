#pragma once

#include <gtest/gtest.h>
#include "base_mock.hpp"

using namespace ::testing;

// Server base test fixture
struct BaseTest : ::testing::Test {
protected:
  BaseTest();
  virtual ~BaseTest();

  void Receive(std::span<uint8_t const> payload);
  void Receive(std::string ip, std::span<uint8_t const> payload);
  void Receive(int sock_fd, std::string ip, std::span<uint8_t const> payload);
  void Execute();

  BaseMock _mock;

  int _sock_fd{1};
  std::string _ip{"192.168.0.1"};
};
