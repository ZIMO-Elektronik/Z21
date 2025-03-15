#pragma once

#include <QLabel>
#include <QWidget>
#include <z21/z21.hpp>
#include "initial_cvs.hpp"

// List of turnouts based on QListWidget
class Turnout : public QWidget, public z21::TurnoutInfo {
  Q_OBJECT

public:
  explicit Turnout(QWidget* parent = nullptr);

public slots:
  // Switching interface
  z21::TurnoutInfo turnoutInfo();
  void turnoutInfo(z21::TurnoutInfo turnout_info);
  virtual void turnout(bool p, bool a, bool);
  z21::TurnoutInfo::Mode turnoutMode();
  void turnoutMode(z21::TurnoutInfo::Mode mode);

  // Programming interface
  uint8_t cvRead(uint16_t cv_addr);
  uint8_t cvWrite(uint16_t cv_addr, uint8_t byte);

private:
  void updateLabel();

  QLabel* _label{new QLabel{this}};
  std::array<uint8_t, 1024uz> _cvs{initial_turnout_cvs};
};

// Overload operator<< for serialization of z21::TurnoutInfo
inline QDataStream& operator<<(QDataStream& stream,
                               z21::TurnoutInfo const& turnout_info) {
  stream << turnout_info.mode << turnout_info.position;
  return stream;
}

// Overload operator>> for deserialization of z21::TurnoutInfo
inline QDataStream& operator>>(QDataStream& stream,
                               z21::TurnoutInfo& turnout_info) {
  z21::TurnoutInfo::Mode mode;
  stream >> mode;
  turnout_info.mode = mode;

  z21::TurnoutInfo::Position position;
  stream >> position;
  turnout_info.position = position;

  return stream;
}

Q_DECLARE_METATYPE(z21::TurnoutInfo);