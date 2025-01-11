#include "loco_list.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include "config.hpp"

// Load locos from config and add them to the list
LocoList::LocoList(QWidget* parent) : QListWidget{parent} {
  // Always focus on the last addressed loco, disable user interaction
  // https://stackoverflow.com/questions/2203698/ignore-mouse-and-keyboard-events-in-qt
  setAttribute(Qt::WA_TransparentForMouseEvents);

  setIconSize(QSize{30, 30});

  Config config;
  auto const size{config.beginReadArray("loco_list")};
  for (auto i{0}; i < size; ++i) {
    config.setArrayIndex(i);
    auto const addr{config.value("address")};
    auto const loco_info{config.value("loco_info")};
    if (!addr.isValid() || !loco_info.isValid()) continue;
    auto list_widget{
      new QListWidgetItem{QPixmap{":/icons/loco.svg"}, addr.toString()}};
    auto loco{new Loco};
    loco->locoInfo(loco_info.value<z21::LocoInfo>());
    addItem(list_widget);
    setItemWidget(list_widget, loco);
  }
  config.endArray();
}

// Store locos in config
LocoList::~LocoList() {
  Config config;
  config.beginWriteArray("loco_list");
  for (auto i{0}; i < count(); ++i) {
    config.setArrayIndex(i);
    auto list_widget{item(i)};
    auto loco{static_cast<Loco*>(itemWidget(list_widget))};
    config.setValue("address", list_widget->text());
    config.setValue("loco_info", QVariant::fromValue(loco->locoInfo()));
  }
  config.endArray();
}

// LAN_X_GET_LOCO_INFO
z21::LocoInfo LocoList::locoInfo(uint16_t loco_addr) {
  return (*this)[loco_addr]->locoInfo();
}

// LAN_X_SET_LOCO_DRIVE | LAN_X_SET_LOCO_E_STOP
void LocoList::locoDrive(uint16_t loco_addr,
                         z21::LocoInfo::SpeedSteps speed_steps,
                         uint8_t rvvvvvvv) {
  auto const before{(*this)[loco_addr]->locoInfo()};
  (*this)[loco_addr]->locoDrive(speed_steps, rvvvvvvv);
  auto const after{(*this)[loco_addr]->locoInfo()};
  if (before != after) emit broadcastLocoInfo(loco_addr);
}

// LAN_X_SET_LOCO_FUNCTION | LAN_X_SET_LOCO_FUNCTION_GROUP
void LocoList::locoFunction(uint16_t loco_addr, uint32_t mask, uint32_t state) {
  auto const before{(*this)[loco_addr]->locoInfo()};
  (*this)[loco_addr]->locoFunction(mask, state);
  auto const after{(*this)[loco_addr]->locoInfo()};
  if (before != after) emit broadcastLocoInfo(loco_addr);
}

// LAN_GET_LOCOMODE
z21::LocoInfo::Mode LocoList::locoMode(uint16_t loco_addr) {
  return (*this)[loco_addr]->locoMode();
}

// LAN_SET_LOCOMODE
void LocoList::locoMode(uint16_t loco_addr, z21::LocoInfo::Mode mode) {
  auto const before{(*this)[loco_addr]->locoMode()};
  (*this)[loco_addr]->locoMode(mode);
  auto const after{(*this)[loco_addr]->locoMode()};
  if (before != after) emit broadcastLocoInfo(loco_addr);
}

// LAN_X_CV_READ
void LocoList::cvRead(uint16_t cv_addr) {
  emit cvAck(cv_addr, _service_loco->cvRead(cv_addr));
}

// LAN_X_CV_WRITE
void LocoList::cvWrite(uint16_t cv_addr, uint8_t byte) {
  emit cvAck(cv_addr, _service_loco->cvWrite(cv_addr, byte));
}

// LAN_X_CV_POM_READ_BYTE
void LocoList::cvPomRead(uint16_t loco_addr, uint16_t cv_addr) {
  emit cvAck(cv_addr, (*this)[loco_addr]->cvRead(cv_addr));
}

// LAN_X_CV_POM_WRITE_BYTE
void LocoList::cvPomWrite(uint16_t loco_addr, uint16_t cv_addr, uint8_t byte) {
  (*this)[loco_addr]->cvWrite(cv_addr, byte);
}

// Access stored locos by subscript operator
Loco* LocoList::operator[](uint16_t loco_addr) {
  // Find loco by address string
  auto const addr_str{QString::number(loco_addr)};
  auto list{findItems(addr_str, Qt::MatchFixedString)};

  // Loco found
  if (std::size(list) == 1uz) {
    auto list_widget{list.first()};
    list_widget->setSelected(true);
    return static_cast<Loco*>(itemWidget(list_widget));
  }
  // Loco not found
  else if (std::empty(list)) {
    // \todo Eventually delete one if size >=256?
    if (count() == 256) assert(false);

    auto list_widget{
      new QListWidgetItem{QPixmap{":/icons/loco.svg"}, addr_str}};
    list_widget->setSelected(true);
    addItem(list_widget);
    auto loco{new Loco};
    setItemWidget(list_widget, loco);
    return loco;
  }
  //
  else
    assert(false);

  std::unreachable();
}
