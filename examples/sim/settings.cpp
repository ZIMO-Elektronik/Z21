#include "settings.hpp"
#include <QApplication>
#include <QDebug>
#include <z21/z21.hpp>

Settings::Settings()
  : QSettings{QCoreApplication::applicationDirPath() + "/../config.ini",
              QSettings::IniFormat} {
  call_once(init_flag_, [this] { init(); });
}

// Register z21::LocoInfo meta type when first used
void Settings::init() { qRegisterMetaType<z21::LocoInfo>(); }