#include "loco_list.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include "settings.hpp"

//
LocoList::LocoList(QWidget* parent) : QListWidget{parent} {
  // Always focus on the last addressed loco, disable user interaction
  // https://stackoverflow.com/questions/2203698/ignore-mouse-and-keyboard-events-in-qt
  setAttribute(Qt::WA_TransparentForMouseEvents);

  setIconSize(QSize{30, 30});

  Settings settings;

  auto const size{settings.beginReadArray("loco_list")};
  for (auto i{0}; i < size; ++i) {
    settings.setArrayIndex(i);

    auto const addr{settings.value("address")};
    auto const loco_info{settings.value("loco_info")};
    if (!addr.isValid() || !loco_info.isValid()) continue;

    auto list_widget{
      new QListWidgetItem{QPixmap{":/icons/loco.svg"}, addr.toString()}};
    auto loco{new Loco};
    loco->locoInfo(loco_info.value<z21::LocoInfo>());
    addItem(list_widget);
    setItemWidget(list_widget, loco);
  }

  settings.endArray();
}

//
LocoList::~LocoList() {
  Settings settings;
  settings.beginWriteArray("loco_list");
  for (auto i{0}; i < count(); ++i) {
    settings.setArrayIndex(i);
    auto list_widget{item(i)};
    auto loco{static_cast<Loco*>(itemWidget(list_widget))};
    settings.setValue("address", list_widget->text());
    settings.setValue("loco_info", QVariant::fromValue(loco->locoInfo()));
  }
  settings.endArray();
}

//
z21::LocoInfo::Mode LocoList::locoMode(uint16_t addr) {
  return (*this)[addr]->locoMode();
}

//
void LocoList::locoMode(uint16_t addr, z21::LocoInfo::Mode mode) {
  (*this)[addr]->locoMode(mode);
}

//
void LocoList::function(uint16_t addr, uint32_t mask, uint32_t state) {
  auto const before{(*this)[addr]->locoInfo()};
  (*this)[addr]->function(mask, state);
  auto const after{(*this)[addr]->locoInfo()};
  if (before != after) emit broadcastLocoInfo(addr);
}

//
void LocoList::drive(uint16_t addr,
                     z21::LocoInfo::SpeedSteps speed_steps,
                     uint8_t rvvvvvvv) {
  auto const before{(*this)[addr]->locoInfo()};
  (*this)[addr]->drive(speed_steps, rvvvvvvv);
  auto const after{(*this)[addr]->locoInfo()};
  if (before != after) emit broadcastLocoInfo(addr);
}

//
z21::LocoInfo LocoList::locoInfo(uint16_t addr) {
  return (*this)[addr]->locoInfo();
}

//
void LocoList::cvRead(uint16_t cv_addr) {
  emit cvAck(cv_addr, _service_loco->cvRead(cv_addr));
}

//
void LocoList::cvWrite(uint16_t cv_addr, uint8_t byte) {
  emit cvAck(cv_addr, _service_loco->cvWrite(cv_addr, byte));
}

//
void LocoList::cvPomRead(uint16_t addr, uint16_t cv_addr) {
  emit cvAck(cv_addr, (*this)[addr]->cvRead(cv_addr));
}

//
void LocoList::cvPomWrite(uint16_t addr, uint16_t cv_addr, uint8_t byte) {
  (*this)[addr]->cvWrite(cv_addr, byte);
}

//
Loco* LocoList::operator[](uint16_t addr) {
  auto const addr_str{QString::number(addr)};
  auto list{findItems(addr_str, Qt::MatchFixedString)};

  //
  if (std::size(list) == 1uz) {
    auto list_widget{list.first()};
    list_widget->setSelected(true);
    auto loco{static_cast<Loco*>(itemWidget(list_widget))};
    return loco;
  }
  //
  else if (std::empty(list)) {
    // delete one if size... above... 256?
    if (count() == 256) {
      asm volatile("nop");
      asm volatile("nop");
    }

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
