#pragma once

#include <QLabel>
#include <QWidget>
#include <z21/z21.hpp>

// Single accessory entry for AccessoryList
class Accessory : public QWidget, public z21::AccessoryInfo {
  Q_OBJECT

public:
  explicit Accessory(QWidget* parent = nullptr);

public slots:
  z21::AccessoryInfo accessoryInfo();
  void accessoryInfo(z21::AccessoryInfo accessory_info);
  void accessory(uint8_t dddddddd);

private:
  void updateLabel();

  QLabel* _label{new QLabel{this}};
};

// Overload operator<< for serialization of z21::AccessoryInfo
inline QDataStream& operator<<(QDataStream& stream,
                               z21::AccessoryInfo const& accessory_info) {
  stream << accessory_info.dddddddd << accessory_info.status;
  return stream;
}

// Overload operator>> for deserialization of z21::AccessoryInfo
inline QDataStream& operator>>(QDataStream& stream,
                               z21::AccessoryInfo& accessory_info) {
  uint8_t dddddddd;
  stream >> dddddddd;
  accessory_info.dddddddd = dddddddd;

  z21::AccessoryInfo::Status status;
  stream >> status;
  accessory_info.status = status;

  return stream;
}

Q_DECLARE_METATYPE(z21::AccessoryInfo);