#include "server.hpp"
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

//
Server::Server(QWidget* parent) : QWidget{parent} {
  auto layout{new QGridLayout};
  layout->setContentsMargins(0, 0, 0, 0);

  _system_tabs->addTab(_system, QPixmap{":/dark-green/computer.svg"}, "System");
  layout->addWidget(_system_tabs, 0, 0, -1, 1);

  _decoders_tabs->addTab(_loco_list, QPixmap{":/icons/loco.svg"}, "Locos");
  _decoders_tabs->addTab(
    new QLabel{"TODO"}, QPixmap{":/icons/turnout.svg"}, "Accessories");
  layout->addWidget(_decoders_tabs, 0, 1, -1, 1);

  _client_log_tabs->addTab(
    _client_log, QPixmap{":/dark-green/file.svg"}, "Client log");
  layout->addWidget(_client_log_tabs, 0, 2, 1, 1);

  auto tabs{new QTabWidget};
  _server_log_tabs->addTab(
    _server_log, QPixmap{":/dark-green/file.svg"}, "Server log");
  layout->addWidget(_server_log_tabs, 1, 2, 1, 1);

  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(1, 1);
  layout->setColumnStretch(2, 2);

  setLayout(layout);

  auto timer{new QTimer{this}};
  connect(timer, &QTimer::timeout, this, &Server::receive);
  timer->start(10);

  //
  connect(_system, &::System::broadcastTrackPowerOff, [this] {
    if (trackPower(false)) broadcastTrackPowerOff();
  });
  connect(_system, &::System::broadcastTrackPowerOn, [this] {
    if (trackPower(true)) broadcastTrackPowerOn();
  });
  connect(_system, &::System::broadcastProgrammingMode, [this] {
    broadcastProgrammingMode();
  });
  connect(_system, &::System::broadcastTrackShortCircuit, [this] {
    broadcastTrackShortCircuit();
  });
  connect(_system, &::System::broadcastStopped, [this] {
    if (stop()) broadcastStopped();
  });
  connect(_loco_list, &::LocoList::broadcastLocoInfo, [this](uint16_t addr) {
    broadcastLocoInfo(addr);
  });
  connect(_system, &::System::broadcastSystemStateData, [this] {
    broadcastSystemStateData();
  });
  connect(_loco_list, &::LocoList::cvNackShortCircuit, [this] {
    cvNackShortCircuit();
  });
  connect(_loco_list, &::LocoList::cvNack, [this] { cvNack(); });
  connect(_loco_list,
          &::LocoList::cvAck,
          [this](uint16_t cv_addr, uint8_t byte) { cvAck(cv_addr, byte); });

  //
  disconnected();
}

//
void Server::receive() {
  sockaddr_in dest_addr_ip4;
  socklen_t socklen{sizeof(dest_addr_ip4)};

  std::array<uint8_t, Z21_MAX_PAYLOAD_SIZE> rx;

  if (auto const len{recvfrom(_sock,
                              std::bit_cast<char*>(std::data(rx)),
                              sizeof(rx) - 1,
                              0,
                              std::bit_cast<sockaddr*>(&dest_addr_ip4),
                              &socklen)};
      len >= 0) {
    ServerBase::receive(
      {_sock, std::bit_cast<sockaddr*>(&dest_addr_ip4), socklen},
      {std::data(rx), static_cast<size_t>(len)});
    execute();
  }

  // Priorities are copied from the app
  if (!_connected) emit ledStatus(Led::Disconnected);
  else if (auto const central_state{systemState().central_state};
           std::to_underlying(central_state &
                              z21::CentralState::ProgrammingModeActive))
    emit ledStatus(Led::ProgrammingMode);
  else if (std::to_underlying(central_state & z21::CentralState::ShortCircuit))
    emit ledStatus(Led::ShortCircuit);
  else if (std::to_underlying(
             (central_state & z21::CentralState::TrackVoltageOff) |
             (central_state & z21::CentralState::EmergencyStop)))
    emit ledStatus(Led::Stop);
  else emit ledStatus(Led::NormalOperation);
}

//
void Server::clearData() { _loco_list->clear(); }

//
void Server::transmit(z21::Socket const& sock,
                      std::span<uint8_t const> datasets) {
  if (sendto(sock.fd,
             std::bit_cast<char*>(std::data(datasets)),
             std::size(datasets),
             0,
             std::bit_cast<sockaddr*>(&sock.addr),
             sock.len) < 0) {
    printf("sendto failed %s\n", strerror(errno));
    std::exit(-1);
  }

  //
  if (!_connected) connected();
}

//
bool Server::trackPower(bool on) { return true; }

//
bool Server::stop() { return true; }

//
void Server::logoff(z21::Socket const&) {
  if (empty(this->clients())) disconnected();
}

//
z21::SystemState& Server::systemState() {
  auto& sys_state{ServerBase::systemState()};
  sys_state.main_current = _system->mainCurrent();
  sys_state.prog_current = _system->progCurrent();
  sys_state.filtered_main_current = _system->filteredMainCurrent();
  sys_state.temperature = _system->temperature();
  sys_state.supply_voltage = _system->supplyVoltage();
  sys_state.vcc_voltage = _system->vccVoltage();
  return ServerBase::systemState();
}

//
void Server::drive(uint16_t addr,
                   z21::LocoInfo::SpeedSteps speed_steps,
                   uint8_t rvvvvvvv) {
  _loco_list->drive(addr, speed_steps, rvvvvvvv);
}

//
z21::LocoInfo::Mode Server::locoMode(uint16_t addr) {
  return _loco_list->locoMode(addr);
}

//
void Server::locoMode(uint16_t addr, z21::LocoInfo::Mode mode) {
  _loco_list->locoMode(addr, mode);
}

//
void Server::function(uint16_t addr, uint32_t mask, uint32_t state) {
  _loco_list->function(addr, mask, state);
}

//
z21::LocoInfo Server::locoInfo(uint16_t addr) {
  return _loco_list->locoInfo(addr);
}

//
bool Server::cvRead(uint16_t cv_addr) {
  emit ledStatus(Led::ProgrammingMode);
  if (!programmingFailure()) _loco_list->cvRead(cv_addr);
  return true;
}

//
bool Server::cvWrite(uint16_t cv_addr, uint8_t byte) {
  emit ledStatus(Led::ProgrammingMode);
  if (!programmingFailure()) _loco_list->cvWrite(cv_addr, byte);
  return true;
}

//
void Server::cvPomRead(uint16_t addr, uint16_t cv_addr) {
  if (!programmingFailure()) _loco_list->cvPomRead(addr, cv_addr);
}

//
void Server::cvPomWrite(uint16_t addr, uint16_t cv_addr, uint8_t byte) {
  if (!programmingFailure()) _loco_list->cvPomWrite(addr, cv_addr, byte);
}

//
void Server::log(char const* str) {
  QString tmp{str};
  tmp.remove(0, 2);
  if (str[0uz] == 'C') _client_log->append(tmp);
  else if (str[0uz] == 'S') _server_log->append(tmp);
  else assert(false);
}

//
bool Server::programmingFailure() {
  if (_system->programmingShortCircuitFailure()) {
    _loco_list->cvNackShortCircuit();
    return true;
  } else if (_system->programmingFailure()) {
    _loco_list->cvNack();
    return true;
  } else return false;
}

//
void Server::connected() {
  _connected = true;
  _system_tabs->setDisabled(false);
  _decoders_tabs->setDisabled(false);
  _client_log_tabs->setDisabled(false);
  _server_log_tabs->setDisabled(false);
  emit ledStatus(Led::Stop);
}

//
void Server::disconnected() {
  _connected = false;
  _system_tabs->setDisabled(true);
  _decoders_tabs->setDisabled(true);
  _client_log_tabs->setDisabled(true);
  _server_log_tabs->setDisabled(true);
  emit ledStatus(Led::Disconnected);
}