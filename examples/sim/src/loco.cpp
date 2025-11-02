#include "loco.hpp"
#include <QGridLayout>
#include <QSpacerItem>

//
Loco::Loco(QWidget* parent) : QWidget{parent} {
  auto layout{new QGridLayout};
  layout->addItem(new QSpacerItem{0, 0}, 0, 0);
  layout->addWidget(_label, 0, 1);
  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(1, 2);
  setLayout(layout);
  updateLabel();
}

// LAN_X_GET_LOCO_INFO
z21::LocoInfo Loco::locoInfo() { return *this; }

// LAN_X_SET_LOCO_INFO (not part of the actual protocol)
void Loco::locoInfo(z21::LocoInfo loco_info) {
  static_cast<z21::LocoInfo&>(*this) = loco_info;
  updateLabel();
}

// LAN_X_SET_LOCO_DRIVE | LAN_X_SET_LOCO_E_STOP
void Loco::locoDrive(z21::LocoInfo::SpeedSteps speed_steps, uint8_t rvvvvvvv) {
  this->speed_steps = speed_steps;
  this->rvvvvvvv = rvvvvvvv;
  updateLabel();
}

// LAN_X_SET_LOCO_FUNCTION | LAN_X_SET_LOCO_FUNCTION_GROUP
void Loco::locoFunction(uint32_t mask, uint32_t state) {
  f31_0 = (~mask & f31_0) | (mask & state);
  updateLabel();
}

// LAN_GET_LOCOMODE
z21::LocoInfo::Mode Loco::locoMode() { return mode; }

// LAN_SET_LOCOMODE
void Loco::locoMode(z21::LocoInfo::Mode mode) {
  this->mode = mode;
  updateLabel();
}

// LAN_X_CV_READ | LAN_X_CV_POM_READ_BYTE
uint8_t Loco::cvRead(uint16_t cv_addr) { return _cvs[cv_addr]; }

// LAN_X_CV_WRITE | LAN_X_CV_POM_WRITE_BYTE
uint8_t Loco::cvWrite(uint16_t cv_addr, uint8_t byte) {
  return _cvs[cv_addr] = byte;
}

// Update label based on current loco info
void Loco::updateLabel() {
  QString text;

  // Not yet supported
  // text += "B" + QString::number(0) + " ";

  if (mode == LocoInfo::Mode::DCC) {
    text += (speed_steps == LocoInfo::DCC14   ? "DCC14 "
             : speed_steps == LocoInfo::DCC28 ? "DCC28 "
                                              : "DCC128") +
            QString{" "};
  } else {
    text += (speed_steps == LocoInfo::DCC14   ? "MMI14 "
             : speed_steps == LocoInfo::DCC28 ? "MMI14 "
                                              : "MMII28") +
            QString{" "};
  }

  text +=
    "R" + QString::number(static_cast<bool>(rvvvvvvv & ztl::mask<7u>)) + " ";

  if (auto const speed{z21::decode_rvvvvvvv(speed_steps, rvvvvvvv)}; !speed)
    text += "Stop";
  else if (speed == -1) text += "EStop";
  else text += "V" + QString::number(speed);
  text += " ";

  text += "F" + QString{"%1"}.arg(f31_0, 8, 16, QLatin1Char('0'));

  _label->setText(text);
}
