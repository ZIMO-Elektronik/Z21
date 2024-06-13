#pragma once

#include <QListWidget>
#include "loco.hpp"

//
class LocoList : public QListWidget {
  Q_OBJECT

public:
  explicit LocoList(QWidget* parent = nullptr);
  ~LocoList();

  // Driving interface
  z21::LocoInfo::Mode locoMode(uint16_t addr);
  void locoMode(uint16_t addr, z21::LocoInfo::Mode mode);
  void function(uint16_t addr, uint32_t mask, uint32_t state);
  void
  drive(uint16_t addr, z21::LocoInfo::SpeedSteps speed_steps, uint8_t rvvvvvvv);
  z21::LocoInfo locoInfo(uint16_t addr);

  // Programming interface
  void cvRead(uint16_t cv_addr);
  void cvWrite(uint16_t cv_addr, uint8_t byte);
  void cvPomRead(uint16_t addr, uint16_t cv_addr);
  void cvPomWrite(uint16_t addr, uint16_t cv_addr, uint8_t byte);

signals:
  void broadcastLocoInfo(uint16_t addr);

  void cvNackShortCircuit();
  void cvNack();
  void cvAck(uint16_t cv_addr, uint8_t byte);

private:
  Loco* operator[](uint16_t addr);

  // Create a separate loco for service mode.This leaks intentionally, we don't
  // want this loco to be part of the actual list.
  Loco* _service_loco{new Loco};
};
