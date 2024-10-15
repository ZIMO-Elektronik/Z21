#include "system.hpp"
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <random>
#include "settings.hpp"

namespace {

// Get random double [0.0, 1.0]
double random_failure() {
  static std::random_device rd;
  static std::mt19937 gen{rd()};
  static std::uniform_real_distribution<> dis{0.0, 1.0};
  return dis(gen);
}

}  // namespace

//
System::System(QWidget* parent) : QWidget{parent} {
  // Layout
  auto layout{new QGridLayout};
  layout->setContentsMargins(11, 0, 11, 11);

  {
    auto groupbox{new QGroupBox{"Broadcasts"}};
    auto grid{new QGridLayout};
    auto track_power_off_button{new QPushButton{"Track power off"}};
    auto track_power_on_button{new QPushButton{"Track power on"}};
    auto programming_mode_button{new QPushButton{"Programming mode"}};
    auto short_circuit_button{new QPushButton{"Short circuit"}};
    auto estop_button{new QPushButton{"E-Stop"}};
    int row{};
    grid->addWidget(new QLabel{"Broadcast track power off"}, row, 0);
    grid->addWidget(track_power_off_button, row++, 1);
    grid->addWidget(new QLabel{"Broadcast track power on"}, row, 0);
    grid->addWidget(track_power_on_button, row++, 1);
    grid->addWidget(new QLabel{"Broadcast programming mode"}, row, 0);
    grid->addWidget(programming_mode_button, row++, 1);
    grid->addWidget(new QLabel{"Broadcast short circuit"}, row, 0);
    grid->addWidget(short_circuit_button, row++, 1);
    grid->addWidget(new QLabel{"Broadcast E-Stop"}, row, 0);
    grid->addWidget(estop_button, row++, 1);
    connect(track_power_off_button,
            &QPushButton::clicked,
            this,
            &System::broadcastTrackPowerOff);
    connect(track_power_on_button,
            &QPushButton::clicked,
            this,
            &System::broadcastTrackPowerOn);
    connect(programming_mode_button,
            &QPushButton::clicked,
            this,
            &System::broadcastProgrammingMode);
    connect(short_circuit_button,
            &QPushButton::clicked,
            this,
            &System::broadcastTrackShortCircuit);
    connect(
      estop_button, &QPushButton::clicked, this, &System::broadcastStopped);
    groupbox->setLayout(grid);
    layout->addWidget(groupbox, 0, 0, 1, 1);
  }

  {
    initFailureRatesSliders();
    auto groupbox{new QGroupBox{"Failure rates"}};
    auto grid{new QGridLayout};
    auto& [labels, values, widgets]{_failure_rates_sliders};
    for (auto i{0uz}; i < std::size(labels); ++i) {
      grid->addWidget(labels[i], i, 0);
      grid->addWidget(values[i], i, 1);
      grid->addWidget(widgets[i], i, 2);
    }
    groupbox->setLayout(grid);
    layout->addWidget(groupbox, 1, 0, 1, 1);
  }

  {
    initCurrentsVoltagesTemperatureSliders();
    auto groupbox{new QGroupBox{"Currents, voltages and temperature"}};
    auto grid{new QGridLayout};
    auto& [labels, values, widgets]{_system_state_sliders};
    for (auto i{0uz}; i < std::size(labels); ++i) {
      grid->addWidget(labels[i], i, 0);
      grid->addWidget(values[i], i, 1);
      grid->addWidget(widgets[i], i, 2);
    }
    groupbox->setLayout(grid);
    layout->addWidget(groupbox, 2, 0, 4, 1);
  }

  setLayout(layout);
}

//
System::~System() {
  Settings settings;

  {
    auto& widgets{get<2uz>(_failure_rates_sliders)};
    settings.setValue("system_prog_short_failure_rate", widgets[0uz]->value());
    settings.setValue("system_prog_failure_rate", widgets[1uz]->value());
  }

  {
    auto& widgets{get<2uz>(_system_state_sliders)};
    settings.setValue("system_main_current", widgets[0uz]->value());
    settings.setValue("system_prog_current", widgets[1uz]->value());
    settings.setValue("system_filtered_main_current", widgets[2uz]->value());
    settings.setValue("system_temperature", widgets[3uz]->value());
    settings.setValue("system_supply_voltage", widgets[4uz]->value());
    settings.setValue("system_vcc_voltage", widgets[5uz]->value());
  }
}

//
bool System::programmingShortCircuitFailure() const {
  auto const& slider{get<2uz>(_failure_rates_sliders)[0uz]};
  return slider->value() &&
         random_failure() <=
           (static_cast<double>(slider->value()) / slider->maximum());
}

//
bool System::programmingFailure() const {
  auto const& slider{get<2uz>(_failure_rates_sliders)[1uz]};
  return slider->value() &&
         random_failure() <=
           (static_cast<double>(slider->value()) / slider->maximum());
}

//
int16_t System::mainCurrent() const {
  return static_cast<int16_t>(get<2uz>(_system_state_sliders)[0uz]->value());
}

//
int16_t System::progCurrent() const {
  return static_cast<int16_t>(get<2uz>(_system_state_sliders)[1uz]->value());
}

//
int16_t System::filteredMainCurrent() const {
  return static_cast<int16_t>(get<2uz>(_system_state_sliders)[2uz]->value());
}

//
int16_t System::temperature() const {
  return static_cast<int16_t>(get<2uz>(_system_state_sliders)[3uz]->value());
}

//
uint16_t System::supplyVoltage() const {
  return static_cast<uint16_t>(get<2uz>(_system_state_sliders)[4uz]->value());
}

//
uint16_t System::vccVoltage() const {
  return static_cast<uint16_t>(get<2uz>(_system_state_sliders)[5uz]->value());
}

//
void System::initFailureRatesSliders() {
  auto& [labels, values, widgets]{_failure_rates_sliders};

  //
  labels.push_back(new QLabel{"Programming short circuit [%]"});
  widgets.push_back(new QSlider{Qt::Horizontal});
  widgets.back()->setMinimum(0);
  widgets.back()->setMaximum(100);
  values.push_back(new QLabel{QString::number(widgets.back()->value())});

  //
  labels.push_back(new QLabel{"Programming [%]"});
  widgets.push_back(new QSlider{Qt::Horizontal});
  widgets.back()->setMinimum(0);
  widgets.back()->setMaximum(100);
  values.push_back(new QLabel{QString::number(widgets.back()->value())});

  //
  for (auto i{0uz}; i < std::size(labels); ++i)
    //
    connect(widgets[i], &QSlider::valueChanged, [=, this](int value) {
      std::get<1uz>(_failure_rates_sliders)[i]->setText(QString::number(value));
    });

  //
  Settings const settings;
  if (auto const value{settings.value("system_prog_short_failure_rate")};
      value.isValid())
    widgets[0uz]->setValue(value.toInt());
  if (auto const value{settings.value("system_prog_failure_rate")};
      value.isValid())
    widgets[1uz]->setValue(value.toInt());
}

//
void System::initCurrentsVoltagesTemperatureSliders() {
  auto& [labels, values, widgets]{_system_state_sliders};

  //
  labels.push_back(new QLabel{"Current main [mA]"});
  widgets.push_back(new QSlider{Qt::Horizontal});
  widgets.back()->setMinimum(0);
  widgets.back()->setMaximum(10000);
  values.push_back(new QLabel{QString::number(widgets.back()->value())});

  //
  labels.push_back(new QLabel{"Current programming [mA]"});
  widgets.push_back(new QSlider{Qt::Horizontal});
  widgets.back()->setMinimum(0);
  widgets.back()->setMaximum(2000);
  values.push_back(new QLabel{QString::number(widgets.back()->value())});

  //
  labels.push_back(new QLabel{"Filtered current main [mA]"});
  widgets.push_back(new QSlider{Qt::Horizontal});
  widgets.back()->setMinimum(0);
  widgets.back()->setMaximum(widgets[0uz]->maximum());
  values.push_back(new QLabel{QString::number(widgets.back()->value())});

  //
  labels.push_back(new QLabel{"Temperature [°C]"});
  widgets.push_back(new QSlider{Qt::Horizontal});
  widgets.back()->setMinimum(0);
  widgets.back()->setMaximum(100);
  values.push_back(new QLabel{QString::number(widgets.back()->value())});

  //
  labels.push_back(new QLabel{"Supply voltage [mV]"});
  widgets.push_back(new QSlider{Qt::Horizontal});
  widgets.back()->setMinimum(12000);
  widgets.back()->setMaximum(widgets.back()->minimum() * 2);
  values.push_back(new QLabel{QString::number(widgets.back()->value())});

  //
  labels.push_back(new QLabel{"VCC voltage [mV]"});
  widgets.push_back(new QSlider{Qt::Horizontal});
  widgets.back()->setMinimum(widgets[4uz]->minimum());
  widgets.back()->setMaximum(widgets[4uz]->maximum() - 1000);
  values.push_back(new QLabel{QString::number(widgets.back()->value())});

  //
  for (auto i{0uz}; i < std::size(labels); ++i) {
    //
    connect(widgets[i], &QSlider::valueChanged, [=, this](int value) {
      std::get<1uz>(_system_state_sliders)[i]->setText(QString::number(value));
    });

    //
    connect(widgets[i],
            &QSlider::sliderReleased,
            this,
            &System::broadcastSystemStateData);
  }

  //
  Settings const settings;
  if (auto const value{settings.value("system_main_current")}; value.isValid())
    widgets[0uz]->setValue(value.toInt());
  if (auto const value{settings.value("system_prog_current")}; value.isValid())
    widgets[1uz]->setValue(value.toInt());
  if (auto const value{settings.value("system_filtered_main_current")};
      value.isValid())
    widgets[2uz]->setValue(value.toInt());
  if (auto const value{settings.value("system_temperature")}; value.isValid())
    widgets[3uz]->setValue(value.toInt());
  if (auto const value{settings.value("system_supply_voltage")};
      value.isValid())
    widgets[4uz]->setValue(value.toInt());
  if (auto const value{settings.value("system_vcc_voltage")}; value.isValid())
    widgets[5uz]->setValue(value.toInt());
}
