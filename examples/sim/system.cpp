#include "system.hpp"
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <random>
#include "config.hpp"

namespace {

// Get random double [0.0, 1.0]
double random_failure() {
  static std::random_device rd;
  static std::mt19937 gen{rd()};
  static std::uniform_real_distribution<> dis{0.0, 1.0};
  return dis(gen);
}

} // namespace

// Load system state from config and create layout
System::System(QWidget* parent) : QWidget{parent} {
  auto layout{new QGridLayout};
  layout->setContentsMargins(11, 0, 11, 11);

  {
    auto groupbox{new QGroupBox{"Broadcasts"}};
    auto grid{new QGridLayout};
    auto track_power_off_button{new QPushButton{"Track power off"}};
    auto track_power_on_button{new QPushButton{"Track power on"}};
    auto programming_mode_button{new QPushButton{"Programming mode"}};
    auto short_circuit_button{new QPushButton{"Short circuit"}};
    auto estop_button{new QPushButton{"EStop"}};
    int row{};
    grid->addWidget(new QLabel{"Broadcast track power off"}, row, 0);
    grid->addWidget(track_power_off_button, row++, 1);
    grid->addWidget(new QLabel{"Broadcast track power on"}, row, 0);
    grid->addWidget(track_power_on_button, row++, 1);
    grid->addWidget(new QLabel{"Broadcast programming mode"}, row, 0);
    grid->addWidget(programming_mode_button, row++, 1);
    grid->addWidget(new QLabel{"Broadcast short circuit"}, row, 0);
    grid->addWidget(short_circuit_button, row++, 1);
    grid->addWidget(new QLabel{"Broadcast EStop"}, row, 0);
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
    initProgramTrack();
    auto groupbox{new QGroupBox{"Program track"}};
    auto grid{new QGridLayout};
    grid->addWidget(new QLabel{"Decoder on program track"}, 0, 0);
    grid->addWidget(_program_track_combobox, 0, 1);
    groupbox->setLayout(grid);
    layout->addWidget(groupbox, 1, 0, 1, 1);
  }

  {
    initFailureRatesWidgets();
    auto groupbox{new QGroupBox{"Failure rates"}};
    auto grid{new QGridLayout};
    grid->setColumnMinimumWidth(1, 3 * 10);
    auto& [labels, values, sliders]{_failure_rates_widgets};
    for (auto i{0uz}; i < std::size(labels); ++i) {
      grid->addWidget(labels[i], static_cast<int>(i), 0);
      grid->addWidget(values[i], static_cast<int>(i), 1);
      grid->addWidget(sliders[i], static_cast<int>(i), 2);
    }
    groupbox->setLayout(grid);
    layout->addWidget(groupbox, 2, 0, 1, 1);
  }

  {
    initCurrentsVoltagesTemperatureWidgets();
    auto groupbox{new QGroupBox{"Currents, voltages and temperature"}};
    auto grid{new QGridLayout};
    grid->setColumnMinimumWidth(1, 5 * 10);
    auto& [labels, values, sliders]{_system_state_widgets};
    for (auto i{0uz}; i < std::size(labels); ++i) {
      grid->addWidget(labels[i], static_cast<int>(i), 0);
      grid->addWidget(values[i], static_cast<int>(i), 1);
      grid->addWidget(sliders[i], static_cast<int>(i), 2);
    }
    groupbox->setLayout(grid);
    layout->addWidget(groupbox, 3, 0, 4, 1);
  }

  setLayout(layout);
}

// Store system state in config
System::~System() {
  Config config;

  {
    auto& sliders{get<2uz>(_failure_rates_widgets)};
    config.setValue("system_prog_short_failure_rate", sliders[0uz]->value());
    config.setValue("system_prog_failure_rate", sliders[1uz]->value());
  }

  {
    config.setValue("system_program_track",
                    _program_track_combobox->currentIndex());
  }

  {
    auto& sliders{get<2uz>(_system_state_widgets)};
    config.setValue("system_main_current", sliders[0uz]->value());
    config.setValue("system_prog_current", sliders[1uz]->value());
    config.setValue("system_filtered_main_current", sliders[2uz]->value());
    config.setValue("system_temperature", sliders[3uz]->value());
    config.setValue("system_supply_voltage", sliders[4uz]->value());
    config.setValue("system_vcc_voltage", sliders[5uz]->value());
  }
}

//
bool System::programmingShortCircuitFailure() const {
  auto const& slider{get<2uz>(_failure_rates_widgets)[0uz]};
  return slider->value() &&
         random_failure() <=
           (static_cast<double>(slider->value()) / slider->maximum());
}

//
bool System::programmingFailure() const {
  auto const& slider{get<2uz>(_failure_rates_widgets)[1uz]};
  return slider->value() &&
         random_failure() <=
           (static_cast<double>(slider->value()) / slider->maximum());
}

//
int System::decoderOnProgrammingTrack() const {
  return _program_track_combobox->currentIndex();
}

//
int16_t System::mainCurrent() const {
  return static_cast<int16_t>(get<2uz>(_system_state_widgets)[0uz]->value());
}

//
int16_t System::progCurrent() const {
  return static_cast<int16_t>(get<2uz>(_system_state_widgets)[1uz]->value());
}

//
int16_t System::filteredMainCurrent() const {
  return static_cast<int16_t>(get<2uz>(_system_state_widgets)[2uz]->value());
}

//
int16_t System::temperature() const {
  return static_cast<int16_t>(get<2uz>(_system_state_widgets)[3uz]->value());
}

//
uint16_t System::supplyVoltage() const {
  return static_cast<uint16_t>(get<2uz>(_system_state_widgets)[4uz]->value());
}

//
uint16_t System::vccVoltage() const {
  return static_cast<uint16_t>(get<2uz>(_system_state_widgets)[5uz]->value());
}

//
void System::initFailureRatesWidgets() {
  auto& [labels, values, sliders]{_failure_rates_widgets};

  //
  labels.push_back(new QLabel{"Programming short circuit [%]"});
  sliders.push_back(new QSlider{Qt::Horizontal});
  sliders.back()->setMinimum(0);
  sliders.back()->setMaximum(100);
  sliders.back()->setStatusTip(
    "Failure rate at which short circuit might occur during programming");
  values.push_back(new QLabel{QString::number(sliders.back()->value())});

  //
  labels.push_back(new QLabel{"Programming [%]"});
  sliders.push_back(new QSlider{Qt::Horizontal});
  sliders.back()->setMinimum(0);
  sliders.back()->setMaximum(100);
  sliders.back()->setStatusTip("Failure rate at which programming might fail");
  values.push_back(new QLabel{QString::number(sliders.back()->value())});

  //
  for (auto i{0uz}; i < std::size(labels); ++i)
    //
    connect(sliders[i], &QSlider::valueChanged, [=, this](int value) {
      std::get<1uz>(_failure_rates_widgets)[i]->setText(QString::number(value));
    });

  //
  Config const config;
  if (auto const value{config.value("system_prog_short_failure_rate")};
      value.isValid())
    sliders[0uz]->setValue(value.toInt());
  if (auto const value{config.value("system_prog_failure_rate")};
      value.isValid())
    sliders[1uz]->setValue(value.toInt());
}

//
void System::initProgramTrack() {
  _program_track_combobox->addItem("Loco");
  _program_track_combobox->addItem("Turnout");
  _program_track_combobox->setStatusTip(
    "Select the decoder type on the program track");

  //
  Config const config;
  if (auto const value{config.value("system_program_track")}; value.isValid())
    _program_track_combobox->setCurrentIndex(value.toInt());
}

//
void System::initCurrentsVoltagesTemperatureWidgets() {
  auto& [labels, values, sliders]{_system_state_widgets};

  //
  labels.push_back(new QLabel{"Current main [mA]"});
  sliders.push_back(new QSlider{Qt::Horizontal});
  sliders.back()->setMinimum(0);
  sliders.back()->setMaximum(10000);
  values.push_back(new QLabel{QString::number(sliders.back()->value())});

  //
  labels.push_back(new QLabel{"Current programming [mA]"});
  sliders.push_back(new QSlider{Qt::Horizontal});
  sliders.back()->setMinimum(0);
  sliders.back()->setMaximum(2000);
  values.push_back(new QLabel{QString::number(sliders.back()->value())});

  //
  labels.push_back(new QLabel{"Filtered current main [mA]"});
  sliders.push_back(new QSlider{Qt::Horizontal});
  sliders.back()->setMinimum(0);
  sliders.back()->setMaximum(sliders[0uz]->maximum());
  values.push_back(new QLabel{QString::number(sliders.back()->value())});

  //
  labels.push_back(new QLabel{"Temperature [°C]"});
  sliders.push_back(new QSlider{Qt::Horizontal});
  sliders.back()->setMinimum(0);
  sliders.back()->setMaximum(100);
  values.push_back(new QLabel{QString::number(sliders.back()->value())});

  //
  labels.push_back(new QLabel{"Supply voltage [mV]"});
  sliders.push_back(new QSlider{Qt::Horizontal});
  sliders.back()->setMinimum(12000);
  sliders.back()->setMaximum(sliders.back()->minimum() * 2);
  values.push_back(new QLabel{QString::number(sliders.back()->value())});

  //
  labels.push_back(new QLabel{"VCC voltage [mV]"});
  sliders.push_back(new QSlider{Qt::Horizontal});
  sliders.back()->setMinimum(sliders[4uz]->minimum());
  sliders.back()->setMaximum(sliders[4uz]->maximum() - 1000);
  values.push_back(new QLabel{QString::number(sliders.back()->value())});

  //
  for (auto i{0uz}; i < std::size(labels); ++i) {
    //
    connect(sliders[i], &QSlider::valueChanged, [=, this](int value) {
      std::get<1uz>(_system_state_widgets)[i]->setText(QString::number(value));
    });

    //
    connect(sliders[i],
            &QSlider::sliderReleased,
            this,
            &System::broadcastSystemStateData);
  }

  //
  Config const config;
  if (auto const value{config.value("system_main_current")}; value.isValid())
    sliders[0uz]->setValue(value.toInt());
  if (auto const value{config.value("system_prog_current")}; value.isValid())
    sliders[1uz]->setValue(value.toInt());
  if (auto const value{config.value("system_filtered_main_current")};
      value.isValid())
    sliders[2uz]->setValue(value.toInt());
  if (auto const value{config.value("system_temperature")}; value.isValid())
    sliders[3uz]->setValue(value.toInt());
  if (auto const value{config.value("system_supply_voltage")}; value.isValid())
    sliders[4uz]->setValue(value.toInt());
  if (auto const value{config.value("system_vcc_voltage")}; value.isValid())
    sliders[5uz]->setValue(value.toInt());
}
