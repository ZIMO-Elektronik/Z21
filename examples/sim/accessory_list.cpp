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
    auto list_widget{
      new QListWidgetItem{QPixmap{":/icons/accessory.svg"}, addr.toString()}};
    auto accessory{new Accessory};
    accessory->accessoryInfo(accessory_info.value<z21::AccessoryInfo>());
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

// LAN_X_GET_EXT_ACCESSORY_INFO
z21::AccessoryInfo AccessoryList::accessoryInfo(uint16_t accy_addr) {
  return (*this)[accy_addr]->accessoryInfo();
}

//
void AccessoryList::accessory(uint16_t accy_addr, uint8_t dddddddd) {
  auto const before{(*this)[accy_addr]->accessoryInfo()};
  (*this)[accy_addr]->accessory(dddddddd);
  auto const after{(*this)[accy_addr]->accessoryInfo()};
  if (before != after) emit broadcastExtAccessoryInfo(accy_addr);
}

// Access stored accessories by subscript operator
Accessory* AccessoryList::operator[](uint16_t accy_addr) {
  // Find accessory by address string
  auto const addr_str{QString::number(accy_addr)};
  auto list{findItems(addr_str, Qt::MatchFixedString)};

  // Accessory found
  if (std::size(list) == 1uz) {
    auto list_widget{list.first()};
    list_widget->setSelected(true);
    return static_cast<Accessory*>(itemWidget(list_widget));
  }
  // Accessory not found
  else if (std::empty(list)) {
    // \todo Eventually delete one if size >=256?
    if (count() == 256) assert(false);

    auto list_widget{
      new QListWidgetItem{QPixmap{":/icons/accessory.svg"}, addr_str}};
    list_widget->setSelected(true);
    addItem(list_widget);
    auto accessory{new Accessory};
    setItemWidget(list_widget, accessory);
    return accessory;
  }
  //
  else
    assert(false);

  std::unreachable();
}