#pragma once

#include <QListWidget>
#include "accessory.hpp"

// List of accessories based on QListWidget
class AccessoryList : public QListWidget {
  Q_OBJECT

public:
  explicit AccessoryList(QWidget* parent = nullptr);
  ~AccessoryList();

  // Switching interface
  z21::TurnoutInfo turnoutInfo(uint16_t accy_addr);
  z21::AccessoryInfo accessoryInfo(uint16_t accy_addr);
  void turnout(uint16_t accy_addr, bool p, bool a, bool q);
  void accessory(uint16_t accy_addr, uint8_t dddddddd);
  z21::TurnoutInfo::Mode turnoutMode(uint16_t accy_addr);
  void turnoutMode(uint16_t accy_addr, z21::TurnoutInfo::Mode mode);

  // Programming interface
  void cvPomAccessoryRead(uint16_t accy_addr, uint16_t cv_addr);
  void cvPomAccessoryWrite(uint16_t accy_addr, uint16_t cv_addr, uint8_t byte);

signals:
  void broadcastTurnoutInfo(uint16_t accy_addr);
  void broadcastExtAccessoryInfo(uint16_t accy_addr);

  void cvNackShortCircuit();
  void cvNack();
  void cvAck(uint16_t cv_addr, uint8_t byte);

private:
  Accessory* accessory(uint16_t accy_addr);
  Accessory* turnout(uint16_t accy_addr);

  template<typename T = z21::TurnoutInfo>
  Accessory* operator[](uint16_t accy_addr, bool change_icon = true);
};
