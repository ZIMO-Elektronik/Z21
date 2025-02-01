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
  z21::TurnoutInfo turnoutInfo();
  void turnoutInfo(z21::TurnoutInfo turnout_info);
  z21::AccessoryInfo accessoryInfo();
  void accessoryInfo(z21::AccessoryInfo accessory_info);
  virtual void turnout(bool p, bool a, bool);
  void accessory(uint8_t dddddddd);
  z21::TurnoutInfo::Mode turnoutMode();
  void turnoutMode(z21::TurnoutInfo::Mode mode);

private:
  void updateLabel();

  QLabel* _label{new QLabel{this}};
};

// Overload operator<< for serialization of z21::AccessoryInfo
inline QDataStream& operator<<(QDataStream& stream,
                               z21::AccessoryInfo const& accessory_info) {
  stream << accessory_info.mode << accessory_info.state
         << accessory_info.dddddddd << accessory_info.status;
  return stream;
}

// Overload operator>> for deserialization of z21::AccessoryInfo
inline QDataStream& operator>>(QDataStream& stream,
                               z21::AccessoryInfo& accessory_info) {
  z21::AccessoryInfo::Mode mode;
  stream >> mode;
  accessory_info.mode = mode;

  z21::AccessoryInfo::State state;
  stream >> state;
  accessory_info.state = state;

  uint8_t dddddddd;
  stream >> dddddddd;
  accessory_info.dddddddd = dddddddd;

  z21::AccessoryInfo::Status status;
  stream >> status;
  accessory_info.status = status;

  return stream;
}

Q_DECLARE_METATYPE(z21::AccessoryInfo);