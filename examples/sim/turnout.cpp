#include "turnout.hpp"
#include <QGridLayout>
#include <QSpacerItem>

//
Turnout::Turnout(QWidget* parent) : QWidget{parent} {
  auto layout{new QGridLayout};

  layout->addItem(new QSpacerItem{0, 0}, 0, 0);
  layout->addWidget(_label, 0, 1);

  layout->setColumnStretch(0, 1);
  layout->setColumnStretch(1, 2);

  setLayout(layout);

  updateLabel();
}

// LAN_X_TURNOUT_INFO
z21::TurnoutInfo Turnout::turnoutInfo() { return *this; }

//
void Turnout::turnoutInfo(z21::TurnoutInfo turnout_info) {
  // Standard layout allows slicing assignment
  static_assert(std::is_standard_layout_v<z21::TurnoutInfo>);
  static_cast<z21::TurnoutInfo&>(*this) = turnout_info;
  updateLabel();
}

// LAN_X_SET_TURNOUT
void Turnout::turnout(bool p, bool a, bool) {
  // Turnout outputs come in pairs and only one can be active at one time. So we
  // can simply ignore any commands which switch outputs off. For a more in
  // depth explanation (in German) see:
  // https://www.kleinbahnsammler.at/wbb2/index.php?thread/28650-zu-z21-app-und-signale/
  if (!a) return;
  state = static_cast<z21::TurnoutInfo::State>(1u << p);
  updateLabel();
}

// LAN_GET_TURNOUTMODE
z21::TurnoutInfo::Mode Turnout::turnoutMode() { return mode; }

// LAN_SET_TURNOUTMODE
void Turnout::turnoutMode(z21::TurnoutInfo::Mode mode) {
  this->mode = mode;
  updateLabel();
}

// Update label based on current turnout info
void Turnout::updateLabel() {
  QString text;

  text += (mode == TurnoutInfo::Mode::DCC ? "DCC" : "MM ") + QString{" "};

  text += state == TurnoutInfo::State::Unknown ? "Unknown"
          : state == TurnoutInfo::State::P0    ? "P0"
          : state == TurnoutInfo::State::P1    ? "P1"
                                               : "Invalid";

  _label->setText(text);
}