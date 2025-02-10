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

  // Programming interface
  void cvRead(uint16_t cv_addr);
  void cvWrite(uint16_t cv_addr, uint8_t byte);
  void cvPomAccessoryRead(uint16_t accy_addr, uint16_t cv_addr);
  void cvPomAccessoryWrite(uint16_t accy_addr, uint16_t cv_addr, uint8_t byte);

signals:
  void broadcastTurnoutInfo(uint16_t accy_addr);

  void cvNackShortCircuit();
  void cvNack();
  void cvAck(uint16_t cv_addr, uint8_t byte);

private:
  Turnout* operator[](uint16_t accy_addr);

  // Create a separate turnout for service mode. This leaks intentionally, we
  // don't want this turnout to be part of the actual list.
  Turnout* _service_turnout{new Turnout};
};
