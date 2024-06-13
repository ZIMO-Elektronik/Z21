#pragma once

#include <QSettings>
#include <QStandardPaths>
#include <memory>
#include <mutex>

// Application settings
//
// Stores a "config.ini" file in the applications root folder.
class Settings : public QSettings {
public:
  Settings();

private:
  static inline std::once_flag init_flag_;

  void init();
};