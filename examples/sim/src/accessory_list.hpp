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
  z21::AccessoryInfo accessoryInfo(uint16_t accy_addr);
  void accessory(uint16_t accy_addr, uint8_t dddddddd);

signals:
  void broadcastExtAccessoryInfo(uint16_t accy_addr);

private:
  Accessory* operator[](uint16_t accy_addr);
};
