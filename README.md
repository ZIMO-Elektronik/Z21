# Z21

[![build](https://github.com/ZIMO-Elektronik/Z21/actions/workflows/build.yml/badge.svg)](https://github.com/ZIMO-Elektronik/Z21/actions/workflows/build.yml) [![tests](https://github.com/ZIMO-Elektronik/Z21/actions/workflows/tests.yml/badge.svg)](https://github.com/ZIMO-Elektronik/Z21/actions/workflows/tests.yml) [![license](https://img.shields.io/github/license/ZIMO-Elektronik/Z21)](https://github.com/ZIMO-Elektronik/Z21/raw/master/LICENSE)

<img src="https://github.com/ZIMO-Elektronik/Z21/raw/master/data/images/logo.png" width="15%" align="right">

The [ROCO](https://www.roco.cc/ren/) [Z21](https://www.z21.eu/en) is a command station with support for LocoNet, R-Bus and XpressNet devices. It has an open LAN interface with a well-documented protocol that has been continuously developed since then. This C++ library of the same name contains **platform-independent** code for the server-side (i.e. the part that runs on a command station) implementation of the protocol.

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#protocol">Protocol</a></li>
    <li><a href="#features">Features</a></li>
    <li><a href="#getting-started">Getting Started</a></li>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#build">Build</a></li>
      </ul>
    <li><a href="#usage">Usage</a></li>
      <ul>
        <li><a href="#server">Server</a></li>
        <li><a href="#interfaces">Interfaces</a></li>
        <li><a href="#commands">Commands</a></li>
        <ul>
          <li><a href="#broadcasts">Broadcasts</a></li>
        </ul>
      </ul>
  </ol>
</details>

## Protocol
The official documentation of the protocol can be downloaded from the ROCO homepage in English and German.

| English                                                                                                                                                                               | German                                                                                                                                                       |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| [Z21 LAN protocol V1.13](https://www.z21.eu/media/Kwc_Basic_DownloadTag_Component/root-en-main_47-1652-959-downloadTag-download/default/d559b9cf/1699290380/z21-lan-protokoll-en.pdf) | [Z21 LAN Protokoll V1.13](https://www.z21.eu/media/Kwc_Basic_DownloadTag_Component/47-1652-959-downloadTag/default/69bad87e/1699290251/z21-lan-protokoll.pdf)|

## Features
- Platform-independent
- Fine grained [interfaces](#interfaces) ( :ballot_box_with_check: done / :negative_squared_cross_mark: todo)
  - Can :negative_squared_cross_mark:
  - Driving :ballot_box_with_check:
  - FastClock :negative_squared_cross_mark:
  - LocoNet :negative_squared_cross_mark:
  - Programming :ballot_box_with_check:
  - RailCom :ballot_box_with_check:
  - RBus :negative_squared_cross_mark:
  - Settings :ballot_box_with_check:
  - Switching :ballot_box_with_check:
  - System :ballot_box_with_check:
  - ZLink :negative_squared_cross_mark:
  - Logging :ballot_box_with_check:
- Examples for Linux, Windows and ESP32

## Getting Started

### Prerequisites
- C++23 compatible compiler
- [CMake](https://cmake.org/) ( >= 3.25 )
- Optional
  - for building [ESP32](https://www.espressif.com/en/products/socs/esp32) example
    - [ESP-IDF](https://github.com/espressif/esp-idf) ( >= 5.0.3 )

### Installation
This library is meant to be consumed with CMake,

```cmake
# Either by including it with CPM
cpmaddpackage("gh:ZIMO-Elektronik/Z21@0.3.3")

# or the FetchContent module
FetchContent_Declare(
  DCC
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/Z21"
  GIT_TAG v0.3.3)

target_link_libraries(YourTarget PRIVATE Z21::Z21)
```

or, on [ESP32 platforms](https://www.espressif.com/en/products/socs/esp32), with the [IDF Component Manager](https://docs.espressif.com/projects/idf-component-manager/en/latest/) by adding it to a `idf_component.yml` file.
```yaml
dependencies:
  zimo-elektronik/z21:
    version: "0.3.3"
```

### Build
The library itself is header-only, so technically it can't be built. However, if run as top-level CMake project then, depending on the target platform, different examples can be built.

#### Host
On host platforms a simulator example running a Z21 server can be built.
```sh
cmake -Bbuild
cmake --build build --target Z21Sim
```

The simulator allows you to try out the library directly in a simple GUI. The application starts a Z21 server to which you can connect directly via [Z21 app](https://www.z21.eu/en/products/z21-app). You can then control locomotives, switch turnouts or simulate various errors such as a short circuit.
![Z21Sim](https://github.com/ZIMO-Elektronik/Z21/raw/master/data/images/Z21Sim.png)

#### ESP32
On [ESP32 platforms](https://www.espressif.com/en/products/socs/esp32) examples from the [examples](https://github.com/ZIMO-Elektronik/DCC/raw/master/examples) subfolder can be built directly using the [IDF Frontend](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/tools/idf-py.html).

```sh
idf.py create-project-from-example "zimo-elektronik/z21^0.3.3:esp32"
```

## Usage

### Server
To create a server it is necessary to derive from `z21::server::Base`. The class uses a mixture of static and dynamic polymorphism, where the desired interfaces must be passed as template arguments, but the actual implementations of these interfaces are classic virtual methods. The interfaces have no dependencies on each other and can be freely selected according to requirements. The only exception to this is the [System interface](#system), which is **mandatory** because it covers basic functions of the protocol. An implementation of such an (almost) minimal version can be found in the [POSIX/ESP32 example](https://github.com/ZIMO-Elektronik/Z21/tree/master/examples/posix).

This example is also an excellent way to see what receiving and transmitting data might look like. The Z21 protocol uses [UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol) over port 21105 or 21106, although control applications on the client should prefer 21105.

#### Receiving
The following snippet shows what a task that reads data via [recvfrom](https://pubs.opengroup.org/onlinepubs/000095399/functions/recvfrom.html) and passes it to the server can look like.
```cpp
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
```

#### Transmitting
Analogous to recvfrom, [sendto](https://pubs.opengroup.org/onlinepubs/009604499/functions/sendto.html) can be used to implement the pure virtual method `transmit` of the [System interface](#system).
```cpp
void Server::transmit(z21::Socket const& sock,
                      std::span<uint8_t const> datasets) {
  // Transmit UDP datasets using sendto
  if (auto const len{sendto(sock.fd,
                            std::bit_cast<char*>(data(datasets)),
                            size(datasets),
                            0,
                            std::bit_cast<sockaddr*>(&sock.addr),
                            sock.len)};
      len < 0) {
    fprintf(stderr, "sendto failed %s", strerror(errno));
    std::exit(-1);
  }
}
```

### Interfaces
| Interface                        | Responsibility                                                         |
| -------------------------------- | ---------------------------------------------------------------------- |
| [Can](#can)                      | Send and receive messages from CAN occupancy detectors                 |
| [Driving](#driving)              | Control mobile decoders                                                |
| [FastClock](#fastclock)          | Control fast clock                                                     |
| [LocoNet](#loconet)              | Send and receive messages from LocoNet gateway                         |
| [Programming](#programming)      | Reading and writing decoder CVs in service or POM mode                 |
| [RailCom](#railcom)              | Reading RailCom data of mobile decoders                                |
| [RBus](#rbus)                    | Send and receive messages from R-Bus feedback modules                  |
| [Settings](#settings)            | Change persistently stored settings                                    |
| [Switching](#switching)          | Control stationary decoders                                            |
| [System](#system) (**required**) | System features (e.g. track power, get hardware information, ...)      |
| [ZLink](#zlink)                  | Send and receive messages from zLink devices                           |
|                                  |                                                                        |
| [Logging](#logging)              | Print received and transmitted UDP datasets (not part of Z21 protocol) |

#### Can
:construction:

#### Driving
:construction:

#### FastClock
:construction:

#### LocoNet
:construction:

#### Programming
:construction:

#### RailCom
:construction:

#### RBus
:construction:

#### Settings
:construction:

#### Switching
:construction:

#### System
The system interface covers the following commands.
| Command                    | Reply                               | Broadcast                   |
| -------------------------- | ----------------------------------- | --------------------------- |
| LAN_GET_SERIAL_NUMBER      | Reply_to_LAN_GET_SERIAL_NUMBER      | -                           |
| LAN_GET_CODE               | Reply_to_LAN_GET_CODE               | -                           |
| LAN_GET_HWINFO             | Reply_to_LAN_GET_HWINFO             | -                           |
| LAN_LOGOFF                 | -                                   | -                           |
| LAN_X_GET_VERSION          | Reply_to_LAN_X_GET_VERSION          | -                           |
| LAN_X_GET_STATUS           | LAN_X_STATUS_CHANGED                | -                           |
| LAN_X_SET_TRACK_POWER_OFF  | LAN_X_BC_TRACK_POWER_OFF            | LAN_X_BC_TRACK_POWER_OFF    |
| LAN_X_SET_TRACK_POWER_ON   | LAN_X_BC_TRACK_POWER_ON             | LAN_X_BC_TRACK_POWER_ON     |
| LAN_X_GET_TURNOUT_INFO     | LAN_X_TURNOUT_INFO                  | LAN_X_TURNOUT_INFO          |
| LAN_X_SET_STOP             | LAN_X_BC_STOPPED                    | LAN_X_BC_STOPPED            |
| LAN_X_GET_FIRMWARE_VERSION | Reply_to_LAN_X_GET_FIRMWARE_VERSION | -                           |
| LAN_SET_BROADCASTFLAGS     | -                                   | -                           |
| LAN_GET_BROADCASTFLAGS     | Reply_to_LAN_GET_BROADCASTFLAGS     | -                           |
| LAN_SYSTEMSTATE_GETDATA    | LAN_SYSTEMSTATE_DATACHANGED         | LAN_SYSTEMSTATE_DATACHANGED |

As well as the subsequent broadcasts.
| Broadcast                    |
| ---------------------------- |
| LAN_X_BC_TRACK_POWER_OFF     |
| LAN_X_BC_TRACK_POWER_ON      |
| LAN_X_BC_PROGRAMMING_MODE    |
| LAN_X_BC_TRACK_SHORT_CIRCUIT |
| LAN_X_BC_STOPPED             |
| LAN_SYSTEMSTATE_DATACHANGED  |

As mentioned before, the system interface **must** be inherited and implemented. As a minimum requirement, this concerns the method for switching the track voltage on and off, as well as that for the global collective stop.
```cpp
[[nodiscard]] virtual bool trackPower(bool on) = 0;
[[nodiscard]] virtual bool stop() = 0;
```

Optionally, the getter for the serial number can be overridden, where the default implementation returns `0`, as well as the callback for logout events, where the default is a noop.
```cpp
[[nodiscard]] virtual int32_t serialNumber() const { return 0; }
virtual void logoff(Socket const&) {}
```

The getter for the [system state](https://github.com/ZIMO-Elektronik/Z21/tree/master/include/z21/system_state.hpp) occupies a special position. It provides a mutable reference through which the system state can be directly manipulated. This allows for a combination of implicit and explicit state changes at the user's discretion. If this function is not overridden, the library defaults are used, and commands like `LAN_X_SET_TRACK_POWER_ON` simply set or clear the corresponding flags. However, if this function is overridden, users can change these flags or other values, such as track voltage, as desired.
```cpp
[[nodiscard]] virtual SystemState& systemState() { return _sys_state; }
```

> [!TIP]
> A typical override could look something like this, where the current and voltage values of the system state are updated by ADC measurements.
> ```cpp
> z21::SystemState& Server::systemState() {
>   auto& sys_state{ServerBase::systemState()};
>   sys_state.main_current = adc_read_current();
>   sys_state.supply_voltage = adc_read_voltage();
>   return sys_state;
> }
> ```

Also optional is a series of broadcasts, which are [pure virtual](https://en.cppreference.com/w/cpp/language/abstract_class.html) but are already implemented in the base class.
```cpp
virtual void broadcastTrackPowerOff() = 0;
virtual void broadcastTrackPowerOn() = 0;
virtual void broadcastProgrammingMode() = 0;
virtual void broadcastTrackShortCircuit() = 0;
virtual void broadcastStopped() = 0;
virtual void broadcastSystemStateData() = 0;
```

#### ZLink
:construction:

#### Logging
:construction:

## Commands
| Command                           | Reply                                                | Broadcast                   |
| --------------------------------- | ---------------------------------------------------- | --------------------------- |
| LAN_GET_SERIAL_NUMBER             | Reply_to_LAN_GET_SERIAL_NUMBER                       | -                           |
| LAN_GET_COMMON_SETTINGS           | Reply_to_LAN_GET_COMMON_SETTINGS                     | -                           |
| LAN_SET_COMMON_SETTINGS           | -                                                    | -                           |
| LAN_GET_MMDCC_SETTINGS            | Reply_to_LAN_GET_MMDCC_SETTINGS                      | -                           |
| LAN_SET_MMDCC_SETTINGS            | -                                                    | -                           |
| LAN_GET_CODE                      | Reply_to_LAN_GET_CODE                                | -                           |
| LAN_GET_HWINFO                    | Reply_to_LAN_GET_HWINFO                              | -                           |
| LAN_LOGOFF                        | -                                                    | -                           |
| LAN_X_GET_VERSION                 | Reply_to_LAN_X_GET_VERSION                           | -                           |
| LAN_X_GET_STATUS                  | LAN_X_STATUS_CHANGED                                 | -                           |
| LAN_X_SET_TRACK_POWER_OFF         | LAN_X_BC_TRACK_POWER_OFF                             | LAN_X_BC_TRACK_POWER_OFF    |
| LAN_X_SET_TRACK_POWER_ON          | LAN_X_BC_TRACK_POWER_ON                              | LAN_X_BC_TRACK_POWER_ON     |
| LAN_X_DCC_READ_REGISTER (n.a.)    | LAN_X_CV_NACK_SC<br>LAN_X_CV_NACK<br>LAN_X_CV_RESULT | LAN_X_BC_PROGRAMMING_MODE   |
| LAN_X_CV_READ                     | LAN_X_CV_NACK_SC<br>LAN_X_CV_NACK<br>LAN_X_CV_RESULT | LAN_X_BC_PROGRAMMING_MODE   |
| LAN_X_DCC_WRITE_REGISTER (n.a.)   | LAN_X_CV_NACK_SC<br>LAN_X_CV_NACK<br>LAN_X_CV_RESULT | LAN_X_BC_PROGRAMMING_MODE   |
| LAN_X_CV_WRITE                    | LAN_X_CV_NACK_SC<br>LAN_X_CV_NACK<br>LAN_X_CV_RESULT | LAN_X_BC_PROGRAMMING_MODE   |
| LAN_X_MM_WRITE_BYTE (n.a.)        | LAN_X_CV_NACK_SC<br>LAN_X_CV_NACK<br>LAN_X_CV_RESULT | LAN_X_BC_PROGRAMMING_MODE   |
| LAN_X_GET_TURNOUT_INFO            | LAN_X_TURNOUT_INFO                                   | LAN_X_TURNOUT_INFO          |
| LAN_X_GET_EXT_ACCESSORY_INFO      | LAN_X_EXT_ACCESSORY_INFO                             | LAN_X_EXT_ACCESSORY_INFO    |
| LAN_X_SET_TURNOUT                 | -                                                    | LAN_X_TURNOUT_INFO          |
| LAN_X_SET_EXT_ACCESSORY           | -                                                    | LAN_X_EXT_ACCESSORY_INFO    |
| LAN_X_SET_STOP                    | LAN_X_BC_STOPPED                                     | LAN_X_BC_STOPPED            |
| LAN_X_SET_LOCO_E_STOP             | -                                                    | LAN_X_LOCO_INFO             |
| LAN_X_PURGE_LOCO                  | -                                                    |                             |
| LAN_X_GET_LOCO_INFO               | LAN_X_LOCO_INFO                                      |                             |
| LAN_X_SET_LOCO_DRIVE              | -                                                    | LAN_X_LOCO_INFO             |
| LAN_X_SET_LOCO_FUNCTION           | -                                                    | LAN_X_LOCO_INFO             |
| LAN_X_SET_LOCO_FUNCTION_GROUP     | -                                                    | LAN_X_LOCO_INFO             |
| LAN_X_SET_LOCO_BINARY_STATE       | -                                                    |                             |
| LAN_X_CV_POM_WRITE_BYTE           | -                                                    |                             |
| LAN_X_CV_POM_WRITE_BIT            | -                                                    |                             |
| LAN_X_CV_POM_READ_BYTE            | LAN_X_CV_NACK_SC<br>LAN_X_CV_NACK<br>LAN_X_CV_RESULT |                             |
| LAN_X_CV_POM_ACCESSORY_WRITE_BYTE | -                                                    |                             |
| LAN_X_CV_POM_ACCESSORY_WRITE_BIT  | -                                                    |                             |
| LAN_X_CV_POM_ACCESSORY_READ_BYTE  | LAN_X_CV_NACK_SC<br>LAN_X_CV_NACK<br>LAN_X_CV_RESULT |                             |
| LAN_X_GET_FIRMWARE_VERSION        | Reply_to_LAN_X_GET_FIRMWARE_VERSION                  |                             |
| LAN_SET_BROADCASTFLAGS            | -                                                    |                             |
| LAN_GET_BROADCASTFLAGS            | Reply_to_LAN_GET_BROADCASTFLAGS                      |                             |
| LAN_GET_LOCOMODE                  | Reply_to_LAN_GET_LOCOMODE                            |                             |
| LAN_SET_LOCOMODE                  | -                                                    |                             |
| LAN_GET_TURNOUTMODE               | Reply_to_LAN_GET_TURNOUTMODE                         |                             |
| LAN_SET_TURNOUTMODE               | -                                                    |                             |
| LAN_RMBUS_GETDATA                 |                                                      |                             |
| LAN_RMBUS_PROGRAMMODULE           |                                                      |                             |
| LAN_SYSTEMSTATE_GETDATA           | LAN_SYSTEMSTATE_DATACHANGED                          | LAN_SYSTEMSTATE_DATACHANGED |
| LAN_RAILCOM_GETDATA               | LAN_RAILCOM_DATACHANGED                              | LAN_RAILCOM_DATACHANGED     |
| LAN_LOCONET_FROM_LAN              |                                                      |                             |
| LAN_LOCONET_DISPATCH_ADDR         |                                                      |                             |
| LAN_LOCONET_DETECTOR              |                                                      |                             |
| LAN_CAN_DETECTOR                  |                                                      |                             |
| LAN_CAN_DEVICE_GET_DESCRIPTION    |                                                      |                             |
| LAN_CAN_DEVICE_SET_DESCRIPTION    |                                                      |                             |
| LAN_CAN_BOOSTER_SET_TRACKPOWER    |                                                      |                             |
| LAN_FAST_CLOCK_CONTROL            |                                                      |                             |
| LAN_FAST_CLOCK_SETTINGS_GET       |                                                      |                             |
| LAN_FAST_CLOCK_SETTINGS_SET       |                                                      |                             |
| LAN_BOOSTER_SET_POWER             |                                                      |                             |
| LAN_BOOSTER_GET_DESCRIPTION       |                                                      |                             |
| LAN_BOOSTER_SET_DESCRIPTION       |                                                      |                             |
| LAN_BOOSTER_SYSTEMSTATE_GETDATA   |                                                      |                             |
| LAN_DECODER_GET_DESCRIPTION       |                                                      |                             |
| LAN_DECODER_SET_DESCRIPTION       |                                                      |                             |
| LAN_DECODER_SYSTEMSTATE_GETDATA   |                                                      |                             |
| LAN_ZLINK_GET_HWINFO              |                                                      |                             |

## Broadcasts
| Broadcast                               |
| --------------------------------------- |
| LAN_X_TURNOUT_INFO                      |
| LAN_X_EXT_ACCESSORY_INFO                |
| LAN_X_BC_TRACK_POWER_OFF                |
| LAN_X_BC_TRACK_POWER_ON                 |
| LAN_X_BC_PROGRAMMING_MODE               |
| LAN_X_BC_TRACK_SHORT_CIRCUIT            |
| LAN_X_BC_STOPPED                        |
| LAN_X_LOCO_INFO                         |
| LAN_RMBUS_DATACHANGED                   |
| LAN_SYSTEMSTATE_DATACHANGED             |
| LAN_RAILCOM_DATACHANGED                 |
| LAN_CAN_BOOSTER_SYSTEMSTATE_DATACHANGED |
| LAN_BOOSTER_SYSTEMSTATE_DATACHANGED     |
| LAN_DECODER_SYSTEMSTATE_DATACHANGED     |