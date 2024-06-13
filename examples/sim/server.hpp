#pragma once

#include <QTabWidget>
#include <QWidget>
#include <z21/z21.hpp>
#include "get_socket.hpp"
#include "led.hpp"
#include "loco_list.hpp"
#include "log.hpp"
#include "system.hpp"

using ServerBase = z21::server::Base<z21::server::intf::System,
                                     z21::server::intf::Driving,
                                     z21::server::intf::Programming,
                                     z21::server::intf::Logging>;

//
class Server : public QWidget, public ServerBase {
  Q_OBJECT

public:
  explicit Server(QWidget* parent = nullptr);

public slots:
  void clearData();

signals:
  void ledStatus(Led::Status status);

private slots:
  void receive();

private:
  void transmit(z21::Socket const& sock,
                std::span<uint8_t const> datasets) final;

  // System interface
  void trackPower(bool on) final;
  void stop() final;
  virtual int16_t mainCurrent() const final;
  virtual int16_t progCurrent() const final;
  virtual int16_t filteredMainCurrent() const final;
  virtual int16_t temperature() const final;
  virtual uint16_t supplyVoltage() const final;
  virtual uint16_t vccVoltage() const final;

  // Driving interface
  z21::LocoInfo::Mode locoMode(uint16_t addr) final;
  void locoMode(uint16_t addr, z21::LocoInfo::Mode mode) final;
  void function(uint16_t addr, uint32_t mask, uint32_t state) final;
  void drive(uint16_t addr,
             z21::LocoInfo::SpeedSteps speed_steps,
             uint8_t rvvvvvvv) final;
  z21::LocoInfo locoInfo(uint16_t addr) final;

  // Programming interface
  void cvRead(uint16_t cv_addr) final;
  void cvWrite(uint16_t cv_addr, uint8_t byte) final;
  void cvPomRead(uint16_t addr, uint16_t cv_addr) final;
  void cvPomWrite(uint16_t addr, uint16_t cv_addr, uint8_t byte) final;

  // Log interface
  void log(char const* str) final;

  bool programmingFailure();
  void connected();
  void disconnected();

  QTabWidget* _system_tabs{new QTabWidget{this}};
  QTabWidget* _decoders_tabs{new QTabWidget{this}};
  QTabWidget* _client_log_tabs{new QTabWidget{this}};
  QTabWidget* _server_log_tabs{new QTabWidget{this}};

  ::Log* _client_log{new ::Log{this}};
  ::Log* _server_log{new ::Log{this}};
  ::LocoList* _loco_list{new ::LocoList{this}};
  ::System* _system{new ::System{this}};

  int _sock{get_non_blocking_socket()};
  bool _connected{};
};
