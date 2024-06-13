#pragma once

#include <QLabel>
#include <QWidget>
#include <z21/z21.hpp>
#include "initial_loco_cvs.hpp"

//
class Loco : public QWidget, public z21::LocoInfo {
  Q_OBJECT

public:
  explicit Loco(QWidget* parent = nullptr);

public slots:
  z21::LocoInfo::Mode locoMode();
  void locoMode(z21::LocoInfo::Mode mode);
  void function(uint32_t mask, uint32_t state);
  void drive(z21::LocoInfo::SpeedSteps speed_steps, uint8_t rvvvvvvv);
  z21::LocoInfo locoInfo();
  void locoInfo(z21::LocoInfo loco_info);
  uint8_t cvRead(uint16_t cv_addr);
  uint8_t cvWrite(uint16_t cv_addr, uint8_t byte);

private:
  void updateLabel();

  QLabel* _label{new QLabel{this}};

  std::array<uint8_t, 1024uz> _cvs{initial_loco_cvs};
};

inline QDataStream& operator<<(QDataStream& stream,
                               z21::LocoInfo const& loco_info) {
  stream << loco_info.speed_steps << loco_info.rvvvvvvv << loco_info.f31_0;
  return stream;
}

inline QDataStream& operator>>(QDataStream& stream, z21::LocoInfo& loco_info) {
  z21::LocoInfo::SpeedSteps speed_steps;
  stream >> speed_steps;
  loco_info.speed_steps = speed_steps;

  uint8_t rvvvvvvv;
  stream >> rvvvvvvv;
  loco_info.rvvvvvvv = rvvvvvvv;

  uint32_t f31_0;
  stream >> f31_0;
  loco_info.f31_0 = f31_0;

  return stream;
}

Q_DECLARE_METATYPE(z21::LocoInfo);