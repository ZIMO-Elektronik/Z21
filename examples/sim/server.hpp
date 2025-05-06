#pragma once

#include <QTabWidget>
#include <QWidget>
#include <z21/z21.hpp>
#include "accessory_list.hpp"
#include "get_socket.hpp"
#include "led.hpp"
#include "loco_list.hpp"
#include "log.hpp"
#include "settings.hpp"
#include "system.hpp"
#include "turnout_list.hpp"

using ServerBase = z21::server::Base<z21::server::intf::System,
                                     z21::server::intf::Dcc,
                                     z21::server::intf::Settings,
                                     z21::server::intf::Logging>;

// Implementation of Z21 server
class Server : public QWidget, public ServerBase {
  Q_OBJECT

public:
  explicit Server(QWidget* parent = nullptr);

public slots:
  void clearData();

signals:
  void ledStatus(Led::Status status);

private:
  void transmit(z21::Socket const& sock,
                std::span<uint8_t const> datasets) final;

  // System interface
  [[nodiscard]] bool trackPower(bool) final;
  [[nodiscard]] bool stop() final;
  [[nodiscard]] int32_t serialNumber() const final;
  void logoff(z21::Socket const&) final;
  [[nodiscard]] z21::SystemState& systemState() final;

  // Driving interface
  [[nodiscard]] z21::LocoInfo locoInfo(uint16_t loco_addr) final;
  void locoDrive(uint16_t loco_addr,
                 z21::LocoInfo::SpeedSteps speed_steps,
                 uint8_t rvvvvvvv) final;
  void locoFunction(uint16_t loco_addr, uint32_t mask, uint32_t state) final;
  [[nodiscard]] z21::LocoInfo::Mode locoMode(uint16_t loco_addr) final;
  void locoMode(uint16_t loco_addr, z21::LocoInfo::Mode mode) final;

  // Switching interface
  [[nodiscard]] z21::TurnoutInfo turnoutInfo(uint16_t accy_addr) final;
  [[nodiscard]] z21::AccessoryInfo accessoryInfo(uint16_t accy_addr) final;
  virtual void turnout(uint16_t accy_addr, bool p, bool a, bool q) final;
  virtual void accessory(uint16_t accy_addr, uint8_t dddddddd) final;
  [[nodiscard]] z21::TurnoutInfo::Mode turnoutMode(uint16_t accy_addr) final;
  void turnoutMode(uint16_t accy_addr, z21::TurnoutInfo::Mode mode) final;

  // Programming interface
  [[nodiscard]] bool cvRead(uint16_t cv_addr) final;
  [[nodiscard]] bool cvWrite(uint16_t cv_addr, uint8_t byte) final;
  void cvPomRead(uint16_t loco_addr, uint16_t cv_addr) final;
  void cvPomWrite(uint16_t loco_addr, uint16_t cv_addr, uint8_t byte) final;
  void cvPomAccessoryRead(uint16_t accy_addr, uint16_t cv_addr) final;
  void
  cvPomAccessoryWrite(uint16_t accy_addr, uint16_t cv_addr, uint8_t byte) final;

  // RailCom interface
  [[nodiscard]] z21::RailComData railComData(uint16_t loco_addr) final;

  // Settings interface
  [[nodiscard]] z21::CommonSettings commonSettings() final;
  void commonSettings(z21::CommonSettings const& common_settings) final;
  [[nodiscard]] z21::MmDccSettings mmDccSettings() final;
  void mmDccSettings(z21::MmDccSettings const& mm_dcc_settings) final;

  // Log interface
  void log(char const* str) final;

  // Utilities
  void receive();
  void updateLedStatus();
  void connected();
  void disconnected();

  // Layout
  QTabWidget* _system_settings_tabs{new QTabWidget{this}};
  QTabWidget* _decoders_tabs{new QTabWidget{this}};
  QTabWidget* _client_log_tabs{new QTabWidget{this}};
  QTabWidget* _server_log_tabs{new QTabWidget{this}};

  // Logs
  ::Log* _client_log{new ::Log{this}};
  ::Log* _server_log{new ::Log{this}};

  ::LocoList* _loco_list{new ::LocoList{this}};
  ::TurnoutList* _turnout_list{new TurnoutList{this}};
  ::AccessoryList* _accessory_list{new AccessoryList{this}};
  ::System* _system{new ::System{this}};
  ::Settings* _settings{new ::Settings{this}};

  int _sock{get_non_blocking_socket()};
  bool _connected{};
};
