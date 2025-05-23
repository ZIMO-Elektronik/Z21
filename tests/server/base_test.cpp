#include "base_test.hpp"
#include "utility.hpp"

BaseTest::BaseTest() {}

BaseTest::~BaseTest() {}

void BaseTest::Receive(std::span<uint8_t const> payload) {
  Receive(_sock_fd, _ip, payload);
}

void BaseTest::Receive(std::string ip, std::span<uint8_t const> payload) {
  Receive(_sock_fd, ip, payload);
}

void BaseTest::Receive(int sock_fd,
                       std::string ip,
                       std::span<uint8_t const> payload) {
  _mock.receive(fd_ip2socket(sock_fd, ip), payload);
}

void BaseTest::Execute() { _mock.execute(); }
