#include "accessory_list.hpp"
#include "config.hpp"

//
AccessoryList::AccessoryList(QWidget* parent) : QListWidget{parent} {
  // Always focus on the last addressed loco, disable user interaction
  // https://stackoverflow.com/questions/2203698/ignore-mouse-and-keyboard-events-in-qt
  setAttribute(Qt::WA_TransparentForMouseEvents);

  setIconSize(QSize{30, 30});

  Config config;
  auto const size{config.beginReadArray("accessory_list")};
  for (auto i{0}; i < size; ++i) {
    config.setArrayIndex(i);
    auto const addr{config.value("address")};
    auto const accessory_info{config.value("accessory_info")};
    if (!addr.isValid() || !accessory_info.isValid()) continue;
    auto accessory{new Accessory};
    accessory->accessoryInfo(accessory_info.value<z21::AccessoryInfo>());
    QPixmap icon{is_ext_accessory(accessory->accessoryInfo())
                   ? ":/icons/accessory.svg"
                   : ":/icons/turnout.svg"};
    auto list_widget{new QListWidgetItem{icon, addr.toString()}};
    addItem(list_widget);
    setItemWidget(list_widget, accessory);
  }
  config.endArray();
}

// Store accessories in config
AccessoryList::~AccessoryList() {
  Config config;
  config.beginWriteArray("accessory_list");
  for (auto i{0}; i < count(); ++i) {
    config.setArrayIndex(i);
    auto list_widget{item(i)};
    auto accessory{static_cast<Accessory*>(itemWidget(list_widget))};
    config.setValue("address", list_widget->text());
    config.setValue("accessory_info",
                    QVariant::fromValue(accessory->accessoryInfo()));
  }
  config.endArray();
}

// LAN_X_GET_TURNOUT_INFO
z21::TurnoutInfo AccessoryList::turnoutInfo(uint16_t accy_addr) {
  return turnout(accy_addr)->turnoutInfo();
}

// LAN_X_GET_EXT_ACCESSORY_INFO
z21::AccessoryInfo AccessoryList::accessoryInfo(uint16_t accy_addr) {
  return accessory(accy_addr)->accessoryInfo();
}

// LAN_X_SET_TURNOUT
void AccessoryList::turnout(uint16_t accy_addr, bool p, bool a, bool q) {
  auto const before{turnout(accy_addr)->turnoutInfo()};
  turnout(accy_addr)->turnout(p, a, q);
  auto const after{turnout(accy_addr)->turnoutInfo()};
  if (before != after) emit broadcastTurnoutInfo(accy_addr);
}

//
void AccessoryList::accessory(uint16_t accy_addr, uint8_t dddddddd) {
  auto const before{accessory(accy_addr)->accessoryInfo()};
  accessory(accy_addr)->accessory(dddddddd);
  auto const after{accessory(accy_addr)->accessoryInfo()};
  if (before != after) emit broadcastExtAccessoryInfo(accy_addr);
}

// LAN_GET_TURNOUTMODE
z21::TurnoutInfo::Mode AccessoryList::turnoutMode(uint16_t accy_addr) {
  return turnout(accy_addr)->turnoutMode();
}

// LAN_SET_TURNOUTMODE
void AccessoryList::turnoutMode(uint16_t accy_addr,
                                z21::TurnoutInfo::Mode mode) {
  auto const before{turnout(accy_addr)->turnoutMode()};
  turnout(accy_addr)->turnoutMode(mode);
  auto const after{turnout(accy_addr)->turnoutMode()};
  if (before != after) emit broadcastTurnoutInfo(accy_addr);
}

// LAN_X_CV_READ
void AccessoryList::cvRead(uint16_t cv_addr) {
  emit cvAck(cv_addr, _service_accessory->cvRead(cv_addr));
}

// LAN_X_CV_WRITE
void AccessoryList::cvWrite(uint16_t cv_addr, uint8_t byte) {
  emit cvAck(cv_addr, _service_accessory->cvWrite(cv_addr, byte));
}

// LAN_X_CV_POM_ACCESSORY_READ_BYTE
void AccessoryList::cvPomAccessoryRead(uint16_t accy_addr, uint16_t cv_addr) {
  emit cvAck(cv_addr, this->operator[]<>(accy_addr, false)->cvRead(cv_addr));
}

// LAN_X_CV_POM_ACCESSORY_WRITE_BYTE
void AccessoryList::cvPomAccessoryWrite(uint16_t accy_addr,
                                        uint16_t cv_addr,
                                        uint8_t byte) {
  this->operator[]<>(accy_addr, false)->cvWrite(cv_addr, byte);
}

// Access stored accessory (and clear turnout state)
Accessory* AccessoryList::accessory(uint16_t accy_addr) {
  auto accessory{this->operator[]<z21::AccessoryInfo>(accy_addr)};
  accessory->position = z21::TurnoutInfo::Position::Invalid;
  return accessory;
}

// Access stored turnout (and clear accessory state)
Accessory* AccessoryList::turnout(uint16_t accy_addr) {
  auto turnout{this->operator[]<z21::TurnoutInfo>(accy_addr)};
  turnout->dddddddd = 0u;
  turnout->status = z21::AccessoryInfo::Status::Unknown;
  return turnout;
}

// Access stored accessories by subscript operator
template<typename T>
Accessory* AccessoryList::operator[](uint16_t accy_addr, bool change_icon) {
  // Find accessory by address string
  auto const addr_str{QString::number(accy_addr)};
  auto list{findItems(addr_str, Qt::MatchFixedString)};

  QPixmap icon{std::same_as<T, z21::AccessoryInfo> ? ":/icons/accessory.svg"
                                                   : ":/icons/turnout.svg"};

  // Accessory found
  if (std::size(list) == 1uz) {
    auto list_widget{list.first()};
    if (change_icon) list_widget->setIcon(icon);
    list_widget->setSelected(true);
    return static_cast<Accessory*>(itemWidget(list_widget));
  }
  // Accessory not found
  else if (std::empty(list)) {
    /// \todo Eventually delete one if size >=256?
    if (count() == 256) assert(false);

    auto accessory{new Accessory};
    auto list_widget{new QListWidgetItem{icon, addr_str}};
    list_widget->setSelected(true);
    addItem(list_widget);
    setItemWidget(list_widget, accessory);
    return accessory;
  }
  //
  else
    assert(false);

  std::unreachable();
}