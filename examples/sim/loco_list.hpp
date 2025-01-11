#pragma once

#include <QListWidget>
#include "loco.hpp"

// List of locos based on QListWidget
class LocoList : public QListWidget {
  Q_OBJECT

public:
  explicit LocoList(QWidget* parent = nullptr);
  ~LocoList();

  // Driving interface
  z21::LocoInfo locoInfo(uint16_t loco_addr);
  void locoDrive(uint16_t loco_addr,
                 z21::LocoInfo::SpeedSteps speed_steps,
                 uint8_t rvvvvvvv);
  void locoFunction(uint16_t loco_addr, uint32_t mask, uint32_t state);
  z21::LocoInfo::Mode locoMode(uint16_t loco_addr);
  void locoMode(uint16_t loco_addr, z21::LocoInfo::Mode mode);

  // Programming interface
  void cvRead(uint16_t cv_addr);
  void cvWrite(uint16_t cv_addr, uint8_t byte);
  void cvPomRead(uint16_t loco_addr, uint16_t cv_addr);
  void cvPomWrite(uint16_t loco_addr, uint16_t cv_addr, uint8_t byte);

signals:
  void broadcastLocoInfo(uint16_t loco_addr);

  void cvNackShortCircuit();
  void cvNack();
  void cvAck(uint16_t cv_addr, uint8_t byte);

private:
  Loco* operator[](uint16_t loco_addr);

  // Create a separate loco for service mode. This leaks intentionally, we don't
  // want this loco to be part of the actual list.
  Loco* _service_loco{new Loco};
};
