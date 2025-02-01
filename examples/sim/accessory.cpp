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

// LAN_X_TURNOUT_INFO
z21::TurnoutInfo Accessory::turnoutInfo() { return *this; }

//
void Accessory::turnoutInfo(z21::TurnoutInfo turnout_info) {
  static_cast<z21::TurnoutInfo&>(*this) = turnout_info;
  updateLabel();
}

// LAN_X_SET_EXT_ACCESSORY
z21::AccessoryInfo Accessory::accessoryInfo() { return *this; }

//
void Accessory::accessoryInfo(z21::AccessoryInfo accessory_info) {
  static_cast<z21::AccessoryInfo&>(*this) = accessory_info;
  updateLabel();
}

// LAN_X_SET_TURNOUT
void Accessory::turnout(bool p, bool a, bool) {
  // Turnout outputs come in pairs and only one can be active at one time. So we
  // can simply ignore any commands which switch outputs off. For a more in
  // depth explanation (in German) see:
  // https://www.kleinbahnsammler.at/wbb2/index.php?thread/28650-zu-z21-app-und-signale/
  if (!a) return;
  state = static_cast<z21::TurnoutInfo::State>(1u << p);
  updateLabel();
}

// LAN_X_SET_EXT_ACCESSORY
void Accessory::accessory(uint8_t dddddddd) {
  this->dddddddd = dddddddd;
  this->status = Valid;
  updateLabel();
}

// LAN_GET_TURNOUTMODE
z21::TurnoutInfo::Mode Accessory::turnoutMode() { return mode; }

// LAN_SET_TURNOUTMODE
void Accessory::turnoutMode(z21::TurnoutInfo::Mode mode) {
  this->mode = mode;
  updateLabel();
}

// Update label based on current accessory info
void Accessory::updateLabel() {
  QString text;

  text += "D" + QString{"%1"}.arg(dddddddd, 2, 16, QLatin1Char('0'));

  _label->setText(text);
}