#include "loco.hpp"
#include <QGridLayout>
#include <QGroupBox>
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

//
z21::LocoInfo::Mode Loco::locoMode() { return mode; }

//
void Loco::locoMode(z21::LocoInfo::Mode mode) { this->mode = mode; }

//
void Loco::function(uint32_t mask, uint32_t state) {
  f31_0 = (~mask & f31_0) | (mask & state);
  updateLabel();
}

//
void Loco::drive(z21::LocoInfo::SpeedSteps speed_steps, uint8_t rvvvvvvv) {
  this->speed_steps = speed_steps;
  this->rvvvvvvv = rvvvvvvv;
  updateLabel();
}

//
z21::LocoInfo Loco::locoInfo() { return *this; }

//
void Loco::locoInfo(z21::LocoInfo loco_info) {
  // Standard layout allows slicing assignment
  static_assert(std::is_standard_layout_v<z21::LocoInfo>);
  static_cast<z21::LocoInfo&>(*this) = loco_info;
  updateLabel();
}

//
uint8_t Loco::cvRead(uint16_t cv_addr) { return _cvs[cv_addr]; }

//
uint8_t Loco::cvWrite(uint16_t cv_addr, uint8_t byte) {
  return _cvs[cv_addr] = byte;
}

//
void Loco::updateLabel() {
  QString text;

  // Not yet supported
  // text += "B" + QString::number(0) + " ";

  text += (speed_steps == LocoInfo::DCC14   ? "DCC14 "
           : speed_steps == LocoInfo::DCC28 ? "DCC28 "
                                            : "DCC128") +
          QString{" "};

  text += "R" +
          QString::number(static_cast<bool>(rvvvvvvv & ztl::make_mask(7u))) +
          " ";

  if (auto const speed{z21::decode_rvvvvvvv(speed_steps, rvvvvvvv)}; !speed)
    text += "Stop";
  else if (speed == -1) text += "EStop";
  else text += "V" + QString::number(speed);
  text += " ";

  text += "F" + QString{"%1"}.arg(f31_0, 8, 16, QLatin1Char('0'));

  _label->setText(text);
}