#include "config.hpp"
#include <QApplication>
#include <QDebug>
#include <z21/z21.hpp>

Config::Config()
  : QSettings{QCoreApplication::applicationDirPath() + "/../config.ini",
              QSettings::IniFormat} {
  call_once(_init_flag, [this] { init(); });
}

// Register z21::LocoInfo meta type when first used
void Config::init() { qRegisterMetaType<z21::LocoInfo>(); }