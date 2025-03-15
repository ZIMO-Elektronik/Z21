# Z21

[![build](https://github.com/ZIMO-Elektronik/Z21/actions/workflows/build.yml/badge.svg)](https://github.com/ZIMO-Elektronik/Z21/actions/workflows/build.yml) [![tests](https://github.com/ZIMO-Elektronik/Z21/actions/workflows/tests.yml/badge.svg)](https://github.com/ZIMO-Elektronik/Z21/actions/workflows/tests.yml) [![license](https://img.shields.io/github/license/ZIMO-Elektronik/Z21)](https://github.com/ZIMO-Elektronik/Z21/raw/master/LICENSE)

<img src="https://github.com/ZIMO-Elektronik/Z21/raw/master/data/images/logo.png" width="15%" align="right">

The [ROCO](https://www.roco.cc/ren/) [Z21](https://www.z21.eu/en) is a command station with support for LocoNet, R-Bus and XpressNet devices. It has an open LAN interface with a well-documented protocol that has been continuously developed since then. This C++ library of the same name contains platform-independent code for the server-side (i.e. the part that runs on a command station) implementation of the protocol.

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
- Fine grained interfaces (implement only what you need)
- Examples for Linux, Windows and ESP32
- Currently implemented [interfaces](#interfaces)
  - Can :negative_squared_cross_mark:
  - Driving :ballot_box_with_check:
  - FastClock :negative_squared_cross_mark:
  - LocoNet :negative_squared_cross_mark:
  - Programming :ballot_box_with_check:
  - RailCom :negative_squared_cross_mark:
  - RBus :negative_squared_cross_mark:
  - Settings :ballot_box_with_check:
  - Switching :ballot_box_with_check:
  - System :ballot_box_with_check:
  - ZLink :negative_squared_cross_mark:
  - Logging :ballot_box_with_check:

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
cpmaddpackage("gh:ZIMO-Elektronik/Z21@0.1.1")

# or the FetchContent module
FetchContent_Declare(
  DCC
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/Z21"
  GIT_TAG v0.1.1)

target_link_libraries(YourTarget PRIVATE Z21::Z21)
```

or, on [ESP32 platforms](https://www.espressif.com/en/products/socs/esp32), with the [IDF Component Manager](https://docs.espressif.com/projects/idf-component-manager/en/latest/) by adding it to a `idf_component.yml` file.
```yaml
dependencies:
  zimo-elektronik/z21:
    version: "0.1.1"
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
idf.py create-project-from-example "zimo-elektronik/z21^0.1.1:esp32"
```

## Usage

### Server
:construction:

### Interfaces

| Interface             | Responsibility                                                         |
| --------------------- | ---------------------------------------------------------------------- |
| Can                   | Send and receive messages from CAN occupancy detectors                 |
| Driving               | Control mobile decoders                                                |
| FastClock             | Control fast clock                                                     |
| LocoNet               | Send and receive messages from LocoNet gateway                         |
| Programming           | Reading and writing decoder CVs in service or POM mode                 |
| RailCom               | Reading RailCom data of mobile decoders                                |
| RBus                  | Send and receive messages from R-Bus feedback modules                  |
| Settings              | Change persistently stored settings                                    |
| Switching             | Control stationary decoders                                            |
| System (**required**) | System features (e.g. track power, get hardware information, ...)      |
| ZLink                 | Send and receive messages from zLink devices                           |
|                       |                                                                        |
| Logging               | Print received and transmitted UDP datasets (not part of Z21 protocol) |

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
:construction:

#### ZLink
:construction:

#### Logging
:construction:

## All Commands
| Command                      | Reply                               | Broadcast                    |
| ---------------------------- | ----------------------------------- | ---------------------------- |
| lanGetSerialNumber           | replyToLanGetSerialNumber           |                              |
| lanGetCommonSettings         | replyToLanGetCommonSettings         |                              |
| lanSetCommonSettings         |                                     |                              |
| lanGetMmDccSettings          | lanGetMmDccSettings                 |                              |
| lanSetMmDccSettings          |                                     |                              |
| lanGetCode                   | replyToLanGetCode                   |                              |
| lanGetHwInfo                 | replyToLanGetHwInfo                 |                              |
| lanLogoff                    | -                                   |                              |
| lanXGetVersion               | replyToLanXGetVersion               |                              |
| lanXGetStatus                | lanXStatusChanged                   |                              |
| lanXSetTrackPowerOff         | lanXBcTrackPowerOff                 | lanXBcTrackPowerOff          |
| lanXSetTrackPowerOn          | lanXBcTrackPowerOn                  | lanXBcTrackPowerOn           |
| lanXDccReadRegister          | lanXCv[NackSc\|Nack\|Result] (n.a.) | lanXBcProgrammingMode (n.a.) |
| lanXCvRead                   | lanXCv[NackSc\|Nack\|Result]        | lanXBcProgrammingMode        |
| lanXDccWriteRegister         | lanXCv[NackSc\|Nack\|Result] (n.a.) | lanXBcProgrammingMode (n.a.) |
| lanXCvWrite                  | lanXCv[NackSc\|Nack\|Result]        | lanXBcProgrammingMode        |
| lanXMmWriteByte (n.a.)       | lanXCv[NackSc\|Nack\|Result] (n.a.) | lanXBcProgrammingMode (n.a.) |
| lanXGetTurnoutInfo           |                                     | lanXTurnoutInfo              |
| lanXGetExtAccessoryInfo      |                                     | lanXExtAccessoryInfo         |
| lanXSetTurnout               |                                     | lanXTurnoutInfo              |
| lanXSetExtAccessory          |                                     | lanXExtAccessoryInfo         |
| lanXSetStop                  | lanXBcStopped                       |                              |
| lanXSetLocoEStop             |                                     | lanXLocoInfo                 |
| lanXPurgeLoco                | -                                   |                              |
| lanXGetLocoInfo              | lanXLocoInfo                        |                              |
| lanXSetLocoDrive             | -                                   | lanXLocoInfo                 |
| lanXSetLocoFunction          | -                                   | lanXLocoInfo                 |
| lanXSetLocoFunctionGroup     | -                                   | lanXLocoInfo                 |
| lanXSetLocoBinaryState       | -                                   |                              |
| lanXCvPom                    | lanXCv[NackSc\|Nack\|Result]        |                              |
| lanXCvPomAccessory           | lanXCv[NackSc\|Nack\|Result]        |                              |
| lanXGetFirmwareVersion       | replyToLanXGetFirmwareVersion       |                              |
| lanSetBroadcastFlags         | -                                   |                              |
| lanGetBroadcastFlags         | replyToLanGetBroadcastFlags         |                              |
| lanGetLocoMode               | replyToLanGetLocoMode               |                              |
| lanSetLocoMode               | -                                   |                              |
| lanGetTurnoutMode            | replyToLanGetTurnoutMode            |                              |
| lanSetTurnoutMode            | -                                   |                              |
| lanRmBusGetData              |                                     |                              |
| lanRmBusProgramModule        |                                     |                              |
| lanSystemStateGetData        | lanSystemStateDataChanged           |                              |
| lanRailComGetData            | lanRailComDataChanged               |                              |
| lanLocoNetFromLan            |                                     |                              |
| lanLocoNetDispatchAddr       |                                     |                              |
| lanLocoNetDetector           |                                     |                              |
| lanCanDetector               |                                     |                              |
| lanCanDeviceGetDescription   |                                     |                              |
| lanCanDeviceSetDescription   |                                     |                              |
| lanCanBoosterSetTrackPower   |                                     |                              |
| lanFastClockControl          |                                     |                              |
| lanFastClockSettingsGet      |                                     |                              |
| lanFastClockSettingsSet      |                                     |                              |
| lanBoosterSetPower           |                                     |                              |
| lanBoosterGetDescription     |                                     |                              |
| lanBoosterSetDescription     |                                     |                              |
| lanBoosterSystemStateGetData |                                     |                              |
| lanDecoderGetDescription     |                                     |                              |
| lanDecoderSetDescription     |                                     |                              |
| lanDecoderSystemStateGetData |                                     |                              |
| lanZLinkGetHwInfo            |                                     |                              |

## Broadcasts
| Broadcast                           |
| ----------------------------------- |
| lanXTurnoutInfo                     |
| lanXExtAccessoryInfo                |
| lanXBcTrackPowerOff                 |
| lanXBcTrackPowerOn                  |
| lanXBcProgrammingMode               |
| lanXBcTrackShortCircuit             |
| lanXBcStopped                       |
| lanXLocoInfo                        |
| lanRmbusDataChanged                 |
| lanSystemStateDataChanged           |
| lanRailComDataChanged               |
| lanCanBoosterSystemStateDataChanged |
| lanBoosterSystemStateDataChanged    |
| lanDecoderSystemStateDataChanged    |