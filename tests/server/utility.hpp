#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <z21/z21.hpp>

z21::Socket fd_ip2socket(int sock_fd, std::string ip);

std::vector<uint8_t>
make_lan_set_broadcastflags_dataset(z21::BroadcastFlags flags);
std::vector<uint8_t> make_lan_set_broadcastflags_dataset(uint32_t flags);

std::vector<uint8_t> make_lan_logoff_dataset();

std::vector<uint8_t> make_lan_x_set_track_power_off_dataset();

std::vector<uint8_t> make_lan_x_set_track_power_on_dataset();