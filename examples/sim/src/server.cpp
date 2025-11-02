#include "server.hpp"
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <chrono>

using namespace std::chrono_literals;

// Create central layout
Server::Server(QWidget* parent) : QWidget{parent} {
  auto layout{new QGridLayout};
  layout->setContentsMargins(0, 0, 0, 0);

  _system_settings_tabs->addTab(
    _system, QPixmap{":/dark-green/computer.svg"}, "System");
  _system_settings_tabs->addTab(
    _settings, QPixmap{":/dark-green/menu.svg"}, "Settings");
  layout->addWidget(_system_settings_tabs, 0, 0, -1, 1);

  _decoders_tabs->addTab(_loco_list, QPixmap{":/icons/loco.svg"}, "Locos");
  _decoders_tabs->addTab(
    _turnout_list, QPixmap{":/icons/turnout.svg"}, "Turnouts");
  _decoders_tabs->addTab(
    _accessory_list, QPixmap{":/icons/accessory.svg"}, "Accessories");
  layout->addWidget(_decoders_tabs, 0, 1, -1, 1);

  _client_log_tabs->addTab(
    _client_log, QPixmap{":/dark-green/file.svg"}, "Client log");
  layout->addWidget(_client_log_tabs, 0, 2, 1, 1);

  _server_log_tabs->addTab(
    _server_log, QPixmap{":/dark-green/file.svg"}, "Server log");
  layout->addWidget(_server_log_tabs, 1, 2, 1, 1);

  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(1, 1);
  layout->setColumnStretch(2, 2);

  setLayout(layout);

  // Read from socket and update LED every 10ms
  auto timer{new QTimer{this}};
  connect(timer, &QTimer::timeout, [this] {
    receive();
    updateLedStatus();
  });
  timer->start(10ms);

  // Connect system broadcasts
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
  connect(_system, &::System::broadcastSystemStateData, [this] {
    broadcastSystemStateData();
  });

  // Connect loco broadcasts
  connect(_loco_list,
          &::LocoList::broadcastLocoInfo,
          [this](uint16_t loco_addr) { broadcastLocoInfo(loco_addr); });
  connect(_loco_list, &::LocoList::cvNackShortCircuit, [this] {
    cvNackShortCircuit();
  });
  connect(_loco_list, &::LocoList::cvNack, [this] { cvNack(); });
  connect(_loco_list,
          &::LocoList::cvAck,
          [this](uint16_t cv_addr, uint8_t byte) { cvAck(cv_addr, byte); });

  // Connect turnout broadcasts
  connect(_turnout_list,
          &::TurnoutList::broadcastTurnoutInfo,
          [this](uint16_t accy_addr) { broadcastTurnoutInfo(accy_addr); });
  connect(_turnout_list, &::TurnoutList::cvNackShortCircuit, [this] {
    cvNackShortCircuit();
  });
  connect(_turnout_list, &::TurnoutList::cvNack, [this] { cvNack(); });
  connect(_turnout_list,
          &::TurnoutList::cvAck,
          [this](uint16_t cv_addr, uint8_t byte) { cvAck(cv_addr, byte); });

  // Connect accessory broadcasts
  connect(_accessory_list,
          &::AccessoryList::broadcastExtAccessoryInfo,
          [this](uint16_t accy_addr) { broadcastExtAccessoryInfo(accy_addr); });

  // Always start disconnected
  disconnected();
}

// Clear any stored data
void Server::clearData() {
  _loco_list->clear();
  _turnout_list->clear();
  _accessory_list->clear();
}

// Transmit to socket
void Server::transmit(z21::Socket const& sock,
                      std::span<uint8_t const> datasets) {
  // Transmit UDP datasets using sendto
  if (auto const len{sendto(sock.fd,
                            std::bit_cast<char*>(std::data(datasets)),
                            std::size(datasets),
                            0,
                            std::bit_cast<sockaddr*>(&sock.addr),
                            sock.len)};
      len < 0) {
    printf("sendto failed %s\n", strerror(errno));
    std::exit(-1);
  }

  // Change connection status if not yet done so
  if (!_connected) connected();
}

// LAN_X_SET_TRACK_POWER_OFF | LAN_X_SET_TRACK_POWER_ON
bool Server::trackPower(bool) { return true; }

// LAN_X_SET_STOP
bool Server::stop() { return true; }

// LAN_GET_SERIAL_NUMBER
int32_t Server::serialNumber() const {
  auto const id{QSysInfo::machineUniqueId()};
  return std::size(id) >= 4
           ? id[3uz] << 24u | id[2uz] << 16u | id[1uz] << 8u | id[0uz] << 0u
           : rand();
}

// LAN_LOGOFF
void Server::logoff(z21::Socket const&) {
  if (empty(this->clients())) disconnected();
}

// LAN_X_GET_STATUS | LAN_SYSTEMSTATE_GETDATA
z21::SystemState& Server::systemState() {
  auto& sys_state{ServerBase::systemState()};
  sys_state.main_current = _system->mainCurrent();
  sys_state.prog_current = _system->progCurrent();
  sys_state.filtered_main_current = _system->filteredMainCurrent();
  sys_state.temperature = _system->temperature();
  sys_state.supply_voltage = _system->supplyVoltage();
  sys_state.vcc_voltage = _system->vccVoltage();
  return sys_state;
}

// LAN_X_GET_LOCO_INFO
z21::LocoInfo Server::locoInfo(uint16_t loco_addr) {
  return _loco_list->locoInfo(loco_addr);
}

// LAN_X_SET_LOCO_FUNCTION | LAN_X_SET_LOCO_FUNCTION_GROUP
void Server::locoFunction(uint16_t loco_addr, uint32_t mask, uint32_t state) {
  _loco_list->locoFunction(loco_addr, mask, state);
}

// LAN_X_SET_LOCO_DRIVE | LAN_X_SET_LOCO_E_STOP
void Server::locoDrive(uint16_t loco_addr,
                       z21::LocoInfo::SpeedSteps speed_steps,
                       uint8_t rvvvvvvv) {
  _loco_list->locoDrive(loco_addr, speed_steps, rvvvvvvv);
}

// LAN_GET_LOCOMODE
z21::LocoInfo::Mode Server::locoMode(uint16_t loco_addr) {
  return _loco_list->locoMode(loco_addr);
}

// LAN_SET_LOCOMODE
void Server::locoMode(uint16_t loco_addr, z21::LocoInfo::Mode mode) {
  _loco_list->locoMode(loco_addr, mode);
}

// LAN_X_GET_TURNOUT_INFO
z21::TurnoutInfo Server::turnoutInfo(uint16_t accy_addr) {
  return _turnout_list->turnoutInfo(accy_addr);
}

// LAN_X_GET_EXT_ACCESSORY_INFO
z21::AccessoryInfo Server::accessoryInfo(uint16_t accy_addr) {
  return _accessory_list->accessoryInfo(accy_addr);
}

// LAN_X_SET_TURNOUT
void Server::turnout(uint16_t accy_addr, bool p, bool a, bool q) {
  _turnout_list->turnout(accy_addr, p, a, q);
}

// LAN_X_SET_EXT_ACCESSORY
void Server::accessory(uint16_t accy_addr, uint8_t dddddddd) {
  _accessory_list->accessory(accy_addr, dddddddd);
}

// LAN_X_GET_TURNOUTMODE
z21::TurnoutInfo::Mode Server::turnoutMode(uint16_t accy_addr) {
  return _turnout_list->turnoutMode(accy_addr);
}

// LAN_X_SET_TURNOUTMODE
void Server::turnoutMode(uint16_t accy_addr, z21::TurnoutInfo::Mode mode) {
  _turnout_list->turnoutMode(accy_addr, mode);
}

// LAN_X_CV_READ
bool Server::cvRead(uint16_t cv_addr) {
  emit ledStatus(Led::ProgrammingMode);
  QTimer::singleShot(200ms, [=, this] {
    if (_system->programmingShortCircuitFailure()) cvNackShortCircuit();
    else if (_system->programmingFailure()) cvNack();
    else if (_system->decoderOnProgrammingTrack() == 0)
      _loco_list->cvRead(cv_addr);
    else _turnout_list->cvRead(cv_addr);
  });
  return true;
}

// LAN_X_CV_WRITE
bool Server::cvWrite(uint16_t cv_addr, uint8_t byte) {
  emit ledStatus(Led::ProgrammingMode);
  QTimer::singleShot(200ms, [=, this] {
    if (_system->programmingShortCircuitFailure()) cvNackShortCircuit();
    else if (_system->programmingFailure()) cvNack();
    else if (_system->decoderOnProgrammingTrack() == 0)
      _loco_list->cvWrite(cv_addr, byte);
    else _turnout_list->cvWrite(cv_addr, byte);
  });
  return true;
}

// LAN_X_CV_POM_READ_BYTE
void Server::cvPomRead(uint16_t loco_addr, uint16_t cv_addr) {
  QTimer::singleShot(200ms, [=, this] {
    if (_system->programmingShortCircuitFailure()) cvNackShortCircuit();
    else if (_system->programmingFailure()) cvNack();
    else _loco_list->cvPomRead(loco_addr, cv_addr);
  });
}

// LAN_X_CV_POM_WRITE_BYTE
void Server::cvPomWrite(uint16_t loco_addr, uint16_t cv_addr, uint8_t byte) {
  QTimer::singleShot(200ms, [=, this] {
    if (_system->programmingShortCircuitFailure()) cvNackShortCircuit();
    else if (_system->programmingFailure()) cvNack();
    else _loco_list->cvPomWrite(loco_addr, cv_addr, byte);
  });
}

// ACCESSORY_READ_BYTE
void Server::cvPomAccessoryRead(uint16_t accy_addr, uint16_t cv_addr, bool) {
  QTimer::singleShot(200ms, [=, this] {
    if (_system->programmingShortCircuitFailure()) cvNackShortCircuit();
    else if (_system->programmingFailure()) cvNack();
    else _turnout_list->cvPomAccessoryRead(accy_addr, cv_addr);
  });
}

// LAN_X_CV_POM_ACCESSORY_WRITE_BYTE
void Server::cvPomAccessoryWrite(uint16_t accy_addr,
                                 uint16_t cv_addr,
                                 uint8_t byte,
                                 bool) {
  QTimer::singleShot(200ms, [=, this] {
    if (_system->programmingShortCircuitFailure()) cvNackShortCircuit();
    else if (_system->programmingFailure()) cvNack();
    else _turnout_list->cvPomAccessoryWrite(accy_addr, cv_addr, byte);
  });
}

// LAN_RAILCOM_GETDATA
z21::RailComData Server::railComData(uint16_t loco_addr) {
  auto const loco_info{_loco_list->locoInfo(loco_addr)};
  auto const speed{static_cast<uint8_t>(std::max(
    z21::decode_rvvvvvvv(loco_info.speed_steps, loco_info.rvvvvvvv), 0))};
  auto const kmh{speed * 2};
  return {
    .loco_address = loco_addr,
    .receive_counter = 0u,
    .error_counter = 0u,
    .options = static_cast<z21::RailComData::Options>(
      (kmh >= 256 ? z21::RailComData::Options::Speed2
                  : z21::RailComData::Options::Speed1) |
      z21::RailComData::Options::QoS),
    .speed = static_cast<uint8_t>(kmh >= 256 ? kmh - 256 : kmh),
    .qos = _system->qosFailureRate(),
  };
}

// LAN_GET_COMMON_SETTINGS
z21::CommonSettings Server::commonSettings() {
  return _settings->commonSettings();
}

// LAN_SET_COMMON_SETTINGS
void Server::commonSettings(z21::CommonSettings const& common_settings) {
  _settings->commonSettings(common_settings);
}

// LAN_GET_MMDCC_SETTINGS
z21::MmDccSettings Server::mmDccSettings() {
  return _settings->mmDccSettings();
}

// LAN_SET_MMDCC_SETTINGS
void Server::mmDccSettings(z21::MmDccSettings const& mm_dcc_settings) {
  _settings->mmDccSettings(mm_dcc_settings);
}

// Receive from socket (periodically called)
void Server::receive() {
  // IPv4 Internet domain socket address
  sockaddr_in dest_addr_ip4;
  socklen_t socklen{sizeof(dest_addr_ip4)};

  // Receive buffer
  std::array<uint8_t, Z21_MAX_PAYLOAD_SIZE> rx;

  // Receive UDP datasets using recvfrom and execute them
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
}

// Update LED status
void Server::updateLedStatus() {
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

// Append string to client or server log
void Server::log(char const* str) {
  QString tmp{str};
  tmp.remove(0, 2);
  if (str[0uz] == 'C') _client_log->append(tmp);
  else if (str[0uz] == 'S') _server_log->append(tmp);
  else assert(false);
}

// Connected, enable UI and update LED status
void Server::connected() {
  _connected = true;
  _system_settings_tabs->setDisabled(false);
  _decoders_tabs->setDisabled(false);
  _client_log_tabs->setDisabled(false);
  _server_log_tabs->setDisabled(false);
  emit ledStatus(Led::Stop);
}

// Disconnected, disable UI and update LED status
void Server::disconnected() {
  _connected = false;
  _system_settings_tabs->setDisabled(true);
  _decoders_tabs->setDisabled(true);
  _client_log_tabs->setDisabled(true);
  _server_log_tabs->setDisabled(true);
  emit ledStatus(Led::Disconnected);
}
