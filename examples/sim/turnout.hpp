#pragma once

#include <QLabel>
#include <QWidget>
#include <z21/z21.hpp>

//
class Turnout : public QWidget, public z21::TurnoutInfo {
  Q_OBJECT

public:
  explicit Turnout(QWidget* parent = nullptr);

public slots:
  z21::TurnoutInfo turnoutInfo();
  void turnoutInfo(z21::TurnoutInfo turnout_info);
  virtual void turnout(bool p, bool a, bool);
  z21::TurnoutInfo::Mode turnoutMode();
  void turnoutMode(z21::TurnoutInfo::Mode mode);

private:
  void updateLabel();

  QLabel* _label{new QLabel{this}};
};

// Overload operator<< for serialization of z21::TurnoutInfo
inline QDataStream& operator<<(QDataStream& stream,
                               z21::TurnoutInfo const& turnout_info) {
  stream << turnout_info.mode << turnout_info.state;
  return stream;
}

// Overload operator>> for deserialization of z21::TurnoutInfo
inline QDataStream& operator>>(QDataStream& stream,
                               z21::TurnoutInfo& turnout_info) {
  z21::TurnoutInfo::Mode mode;
  stream >> mode;
  turnout_info.mode = mode;

  z21::TurnoutInfo::State state;
  stream >> state;
  turnout_info.state = state;

  return stream;
}

Q_DECLARE_METATYPE(z21::TurnoutInfo);