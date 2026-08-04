#pragma once

#include <QLabel>
#include <QWidget>
#include <z21/z21.hpp>
#include "initial_cvs.hpp"

// Single loco entry for LocoList
class Loco : public QWidget, public z21::LocoInfo, public z21::LocoEntry {
  Q_OBJECT

public:
  explicit Loco(QWidget* parent = nullptr);

public slots:
  // Driving interface
  void locoEStop();
  [[nodiscard]] z21::LocoInfo locoInfo();
  void locoInfo(z21::LocoInfo loco_info);
  [[nodiscard]] z21::LocoEntry locoEntry();
  void locoEntry(z21::LocoEntry loco_entry);
  void locoDrive(z21::LocoInfo::SpeedSteps speed_steps, uint8_t rvvvvvvv);
  void locoFunction(uint32_t mask, uint32_t state);
  [[nodiscard]] z21::LocoInfo::Mode locoMode();
  void locoMode(z21::LocoInfo::Mode mode);

  // Programming interface
  uint8_t cvRead(uint16_t cv_addr);
  uint8_t cvWrite(uint16_t cv_addr, uint8_t byte);

private:
  void updateLabel();

  QLabel* _label{new QLabel{this}};
  std::array<uint8_t, 1024uz> _cvs{initial_loco_cvs};
};

// Overload operator<< for serialization of z21::LocoInfo
inline QDataStream& operator<<(QDataStream& stream,
                               z21::LocoInfo const& loco_info) {
  stream << loco_info.mode << loco_info.speed_steps << loco_info.rvvvvvvv
         << loco_info.f31_0;
  return stream;
}

// Overload operator>> for deserialization of z21::LocoInfo
inline QDataStream& operator>>(QDataStream& stream, z21::LocoInfo& loco_info) {
  z21::LocoInfo::Mode mode;
  stream >> mode;
  loco_info.mode = mode;
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

Q_DECLARE_METATYPE(z21::LocoInfo)

// Overload operator<< for serialization of z21::LocoEntry
inline QDataStream& operator<<(QDataStream& stream,
                               z21::LocoEntry const& loco_entry) {
  stream << loco_entry.index << loco_entry.size
         << QString::fromStdString(loco_entry.name);
  return stream;
}

// Overload operator>> for deserialization of z21::LocoEntry
inline QDataStream& operator>>(QDataStream& stream,
                               z21::LocoEntry& loco_entry) {
  uint8_t index;
  stream >> index;
  loco_entry.index = index;
  uint8_t size;
  stream >> size;
  loco_entry.size = size;
  QString name;
  stream >> name;
  loco_entry.name = name.toStdString();
  return stream;
}

Q_DECLARE_METATYPE(z21::LocoEntry)
