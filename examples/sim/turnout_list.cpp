#include "turnout_list.hpp"
#include "config.hpp"

// Load turnouts from config and add them to the list
TurnoutList::TurnoutList(QWidget* parent) : QListWidget{parent} {
  // Always focus on the last addressed loco, disable user interaction
  // https://stackoverflow.com/questions/2203698/ignore-mouse-and-keyboard-events-in-qt
  setAttribute(Qt::WA_TransparentForMouseEvents);

  setIconSize(QSize{30, 30});

  Config config;
  auto const size{config.beginReadArray("turnout_list")};
  for (auto i{0}; i < size; ++i) {
    config.setArrayIndex(i);
    auto const addr{config.value("address")};
    auto const turnout_info{config.value("turnout_info")};
    if (!addr.isValid() || !turnout_info.isValid()) continue;
    auto list_widget{
      new QListWidgetItem{QPixmap{":/icons/turnout.svg"}, addr.toString()}};
    auto turnout{new Turnout};
    turnout->turnoutInfo(turnout_info.value<z21::TurnoutInfo>());
    addItem(list_widget);
    setItemWidget(list_widget, turnout);
  }
  config.endArray();
}

// Store turnouts in config
TurnoutList::~TurnoutList() {
  Config config;
  config.beginWriteArray("turnout_list");
  for (auto i{0}; i < count(); ++i) {
    config.setArrayIndex(i);
    auto list_widget{item(i)};
    auto turnout{static_cast<Turnout*>(itemWidget(list_widget))};
    config.setValue("address", list_widget->text());
    config.setValue("turnout_info",
                    QVariant::fromValue(turnout->turnoutInfo()));
  }
  config.endArray();
}

// LAN_X_GET_TURNOUT_INFO
z21::TurnoutInfo TurnoutList::turnoutInfo(uint16_t accy_addr) {
  return (*this)[accy_addr]->turnoutInfo();
}

// LAN_X_SET_TURNOUT
void TurnoutList::turnout(uint16_t accy_addr, bool p, bool a, bool q) {
  auto const before{(*this)[accy_addr]->turnoutInfo()};
  (*this)[accy_addr]->turnout(p, a, q);
  auto const after{(*this)[accy_addr]->turnoutInfo()};
  if (before != after) emit broadcastTurnoutInfo(accy_addr);
}

// LAN_GET_TURNOUTMODE
z21::TurnoutInfo::Mode TurnoutList::turnoutMode(uint16_t accy_addr) {
  return (*this)[accy_addr]->turnoutMode();
}

// LAN_SET_TURNOUTMODE
void TurnoutList::turnoutMode(uint16_t accy_addr, z21::TurnoutInfo::Mode mode) {
  auto const before{(*this)[accy_addr]->turnoutMode()};
  (*this)[accy_addr]->turnoutMode(mode);
  auto const after{(*this)[accy_addr]->turnoutMode()};
  if (before != after) emit broadcastTurnoutInfo(accy_addr);
}

// LAN_X_CV_READ
void TurnoutList::cvRead(uint16_t cv_addr) {
  emit cvAck(cv_addr, _service_turnout->cvRead(cv_addr));
}

// LAN_X_CV_WRITE
void TurnoutList::cvWrite(uint16_t cv_addr, uint8_t byte) {
  emit cvAck(cv_addr, _service_turnout->cvWrite(cv_addr, byte));
}

// LAN_X_CV_POM_ACCESSORY_READ_BYTE
void TurnoutList::cvPomAccessoryRead(uint16_t accy_addr, uint16_t cv_addr) {
  emit cvAck(cv_addr, (*this)[accy_addr]->cvRead(cv_addr));
}

// LAN_X_CV_POM_ACCESSORY_WRITE_BYTE
void TurnoutList::cvPomAccessoryWrite(uint16_t accy_addr,
                                      uint16_t cv_addr,
                                      uint8_t byte) {
  (*this)[accy_addr]->cvWrite(cv_addr, byte);
}

// Access stored turnouts by subscript operator
Turnout* TurnoutList::operator[](uint16_t accy_addr) {
  // Find turnout by address string
  auto const addr_str{QString::number(accy_addr)};
  auto list{findItems(addr_str, Qt::MatchFixedString)};

  // Turnout found
  if (std::size(list) == 1uz) {
    auto list_widget{list.first()};
    list_widget->setSelected(true);
    return static_cast<Turnout*>(itemWidget(list_widget));
  }
  // Turnout not found
  else if (std::empty(list)) {
    /// \todo Eventually delete one if size >=256?
    if (count() == 256) assert(false);

    auto list_widget{
      new QListWidgetItem{QPixmap{":/icons/turnout.svg"}, addr_str}};
    list_widget->setSelected(true);
    addItem(list_widget);
    auto turnout{new Turnout};
    setItemWidget(list_widget, turnout);
    return turnout;
  }
  // Turnout found multiple times, shouldn't happen
  else
    assert(false);

  std::unreachable();
}