#pragma once

#include <QSettings>
#include <QStandardPaths>
#include <memory>
#include <mutex>

// Application settings
//
// Stores a "config.ini" file in the applications root folder.
class Config : public QSettings {
public:
  Config();

private:
  static inline std::once_flag _init_flag;

  void init();
};
