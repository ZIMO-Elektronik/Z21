#include "accessory.hpp"
#include <QGridLayout>
#include <QSpacerItem>

//
Accessory::Accessory(QWidget* parent) : QWidget{parent} {
  auto layout{new QGridLayout};
  layout->addItem(new QSpacerItem{0, 0}, 0, 0);
  layout->addWidget(_label, 0, 1);
  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(1, 2);
  setLayout(layout);
  updateLabel();
}

// LAN_X_GET_EXT_ACCESSORY_INFO
z21::AccessoryInfo Accessory::accessoryInfo() { return *this; }

// LAN_X_SET_EXT_ACCESSORY_INFO (not part of the actual protocol)
void Accessory::accessoryInfo(z21::AccessoryInfo accessory_info) {
  static_cast<z21::AccessoryInfo&>(*this) = accessory_info;
  updateLabel();
}

// LAN_X_SET_EXT_ACCESSORY
void Accessory::accessory(uint8_t dddddddd) {
  this->dddddddd = dddddddd;
  this->status = z21::AccessoryInfo::Status::Valid;
  updateLabel();
}

// Update label based on current accessory info
void Accessory::updateLabel() {
  QString text;

  text += "D" + QString{"%1"}.arg(dddddddd, 2, 16, QLatin1Char('0'));

  _label->setText(text);
}
