#include "config.hpp"
#include <QApplication>
#include <QDebug>
#include <z21/z21.hpp>

Config::Config()
  : QSettings{QCoreApplication::applicationDirPath() + "/../config.ini",
              QSettings::IniFormat} {
  call_once(_init_flag, [this] { init(); });
}

// Register z21::AccessoryInfo, z21::LocoInfo, z21::LocoEntry and
// z21::TurnoutInfo meta types when first used
void Config::init() {
  qRegisterMetaType<z21::AccessoryInfo>();
  qRegisterMetaType<z21::LocoInfo>();
  qRegisterMetaType<z21::LocoEntry>();
  qRegisterMetaType<z21::TurnoutInfo>();
}
