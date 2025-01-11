#pragma once

#include <QListWidget>
#include "turnout.hpp"

// List of turnouts based on QListWidget
class TurnoutList : public QListWidget {
  Q_OBJECT

public:
  explicit TurnoutList(QWidget* parent = nullptr);
  ~TurnoutList();

  // Switching interface
  z21::TurnoutInfo turnoutInfo(uint16_t accy_addr);
  void turnout(uint16_t accy_addr, bool p, bool a, bool q);
  z21::TurnoutInfo::Mode turnoutMode(uint16_t accy_addr);
  void turnoutMode(uint16_t accy_addr, z21::TurnoutInfo::Mode mode);

signals:
  void broadcastTurnoutInfo(uint16_t accy_addr);

private:
  Turnout* operator[](uint16_t accy_addr);
};
