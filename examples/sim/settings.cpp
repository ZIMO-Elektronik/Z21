#include "settings.hpp"
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <random>
#include "config.hpp"

// Load settings from config and create layout
Settings::Settings(QWidget* parent) : QWidget{parent} {
  auto layout{new QGridLayout};
  layout->setContentsMargins(11, 0, 11, 11);

  {
    initCommonWidgets();
    auto groupbox{new QGroupBox{"Common"}};
    auto grid{new QGridLayout};
#if !defined(Z21_SIM_HIDE_CONFIDENTIAL_SETTINGS)
    grid->setColumnMinimumWidth(1, 5 * 10);
#endif
    int row{};
    grid->addWidget(new QLabel{"Activate RailCom"}, row, 0);
    grid->addWidget(_common.enable_railcom_checkbox, row++, 2);
    grid->addWidget(new QLabel{"CV29 automatic address"}, row, 0);
    grid->addWidget(
      _common.enable_bit_modify_on_long_address_checkbox, row++, 2);
    grid->addWidget(new QLabel{"Function of the Z21 button"}, row, 0);
    grid->addWidget(_common.key_stop_mode_combobox, row++, 2);
    grid->addWidget(new QLabel{"Programming type"}, row, 0);
    grid->addWidget(_common.programming_type_combobox, row++, 2);
#if !defined(Z21_SIM_HIDE_CONFIDENTIAL_SETTINGS)
    grid->addWidget(new QLabel{"Enable LocoNet current source"}, row, 0);
    grid->addWidget(_common.enable_loconet_current_source_checkbox, row++, 2);
    grid->addWidget(new QLabel{"LocoNet fast clock rate"}, row, 0);
    grid->addWidget(_common.loconet_fast_clock_rate_label, row, 1);
    grid->addWidget(_common.loconet_fast_clock_rate_slider, row++, 2);
    grid->addWidget(new QLabel{"LocoNet mode"}, row, 0);
    grid->addWidget(_common.loconet_mode_combobox, row++, 2);
#endif
    grid->addWidget(new QLabel{"Ext disable turnout timeout"}, row, 0);
    grid->addWidget(
      _common.ext_flags.disable_turnout_timeout_checkbox, row++, 2);
    grid->addWidget(new QLabel{"Ext disable turnout auto deactivate"}, row, 0);
    grid->addWidget(
      _common.ext_flags.disable_turnout_auto_deact_checkbox, row++, 2);
    grid->addWidget(new QLabel{"Ext RCN-213 turnout addressing"}, row, 0);
    grid->addWidget(
      _common.ext_flags.accessory_start_group1_checkbox, row++, 2);
    grid->addWidget(new QLabel{"Ext R-Bus as second X-Bus"}, row, 0);
    grid->addWidget(_common.ext_flags.rbus_as_xbus2_checkbox, row++, 2);
    grid->addWidget(new QLabel{"Ext invert accessory green/red"}, row, 0);
    grid->addWidget(
      _common.ext_flags.invert_accessory_red_green_checkbox, row++, 2);
#if !defined(Z21_SIM_HIDE_CONFIDENTIAL_SETTINGS)
    grid->addWidget(new QLabel{"Purging time"}, row, 0);
    grid->addWidget(_common.purging_time_combobox, row++, 2);
    grid->addWidget(new QLabel{"Bus disable CAN emulates R-Bus"}, row, 0);
    grid->addWidget(_common.bus_flags.disable_emulate_rm_checkbox, row++, 2);
    grid->addWidget(new QLabel{"Bus disable CAN emulates LocoNet"}, row, 0);
    grid->addWidget(_common.bus_flags.disable_emulate_ln_checkbox, row++, 2);
    grid->addWidget(new QLabel{"Bus enable CAN emulates Lissy"}, row, 0);
    grid->addWidget(_common.bus_flags.enable_emulate_lissy_checkbox, row++, 2);
#endif
    grid->addWidget(new QLabel{"Bus X-Bus uses fallback version"}, row, 0);
    grid->addWidget(
      _common.bus_flags.xbus_use_fallback_version_checkbox, row++, 2);
    grid->addWidget(new QLabel{"Bus X-Bus forwarding R-Bus"}, row, 0);
    grid->addWidget(_common.bus_flags.xbus_forwarding_rbus_checkbox, row++, 2);
    grid->addWidget(new QLabel{"Bus only accessories"}, row, 0);
    grid->addWidget(_common.bus_flags.bus_only_accessories_checkbox, row++, 2);
    groupbox->setLayout(grid);
    layout->addWidget(groupbox, 0, 0, 1, 1);
  }

  {
    initMmDccWidgets();
    auto groupbox{new QGroupBox{"MM/DCC"}};
    auto grid{new QGridLayout};
    grid->setColumnMinimumWidth(1, 5 * 10);
    int row{};
    grid->addWidget(new QLabel{"Startup reset package count"}, row, 0);
    grid->addWidget(_mm_dcc.startup_reset_package_count_label, row, 1);
    grid->addWidget(_mm_dcc.startup_reset_package_count_slider, row++, 2);
    grid->addWidget(new QLabel{"Continue reset package count"}, row, 0);
    grid->addWidget(_mm_dcc.continue_reset_package_count_label, row, 1);
    grid->addWidget(_mm_dcc.continue_reset_package_count_slider, row++, 2);
    grid->addWidget(new QLabel{"Program package count"}, row, 0);
    grid->addWidget(_mm_dcc.program_package_count_label, row, 1);
    grid->addWidget(_mm_dcc.program_package_count_slider, row++, 2);
    grid->addWidget(new QLabel{"Bit verify to"}, row, 0);
    grid->addWidget(_mm_dcc.bit_verify_to_one_label, row, 1);
    grid->addWidget(_mm_dcc.bit_verify_to_one_slider, row++, 2);
    grid->addWidget(new QLabel{"Programming ACK current [mA]"}, row, 0);
    grid->addWidget(_mm_dcc.programming_ack_current_label, row, 1);
    grid->addWidget(_mm_dcc.programming_ack_current_slider, row++, 2);
    grid->addWidget(new QLabel{"Flags format"}, row, 0);
    grid->addWidget(_mm_dcc.flags.fmt_combobox, row++, 2);
    grid->addWidget(new QLabel{"Flags refresh high functions"}, row, 0);
    grid->addWidget(_mm_dcc.flags.repeat_hfx_checkbox, row++, 2);
    grid->addWidget(new QLabel{"Flags short loco address"}, row, 0);
    grid->addWidget(_mm_dcc.flags.short_combobox, row++, 2);
#if !defined(Z21_SIM_HIDE_CONFIDENTIAL_SETTINGS)
    grid->addWidget(new QLabel{"Output voltage [mV]"}, row, 0);
    grid->addWidget(_mm_dcc.output_voltage_label, row, 1);
    grid->addWidget(_mm_dcc.output_voltage_slider, row++, 2);
#endif
    grid->addWidget(new QLabel{"Programming voltage [mV]"}, row, 0);
    grid->addWidget(_mm_dcc.programming_voltage_label, row, 1);
    grid->addWidget(_mm_dcc.programming_voltage_slider, row++, 2);
    groupbox->setLayout(grid);
    layout->addWidget(groupbox, 1, 0, 1, 1);
  }

  setLayout(layout);
}

// Store settings in config
Settings::~Settings() {
  Config config;

  {
    config.setValue("settings_enable_railcom",
                    _common.enable_railcom_checkbox->isChecked());
    config.setValue(
      "settings_enable_bit_modify_on_long_address",
      _common.enable_bit_modify_on_long_address_checkbox->isChecked());
    config.setValue("settings_key_stop_mode",
                    _common.key_stop_mode_combobox->currentIndex());
    config.setValue("settings_programming_type",
                    _common.programming_type_combobox->currentIndex());
    config.setValue(
      "settings_enable_loconet_current_source",
      _common.enable_loconet_current_source_checkbox->isChecked());
    config.setValue("settings_loconet_fast_clock_rate",
                    _common.loconet_fast_clock_rate_slider->value());
    config.setValue("settings_loconet_mode",
                    _common.loconet_mode_combobox->currentIndex());
    config.setValue("settings_ext_flags", commonExtFlags());
    config.setValue("settings_purging_time",
                    _common.purging_time_combobox->currentIndex());
    config.setValue("settings_bus_flags", commonBusFlags());
  }

  {
    config.setValue("settings_startup_reset_package_count",
                    _mm_dcc.startup_reset_package_count_slider->value());
    config.setValue("settings_continue_reset_package_count",
                    _mm_dcc.continue_reset_package_count_slider->value());
    config.setValue("settings_program_package_count",
                    _mm_dcc.program_package_count_slider->value());
    config.setValue("settings_bit_verify_to_one",
                    _mm_dcc.bit_verify_to_one_slider->value());
    config.setValue("settings_programming_ack_current",
                    _mm_dcc.programming_ack_current_slider->value());
    config.setValue("settings_mm_dcc_flags", mmDccFlags());
    config.setValue("settings_output_voltage",
                    _mm_dcc.output_voltage_slider->value());
    config.setValue("settings_programming_voltage",
                    _mm_dcc.programming_voltage_slider->value());
  }
}

//
z21::CommonSettings Settings::commonSettings() {
  return {.enable_railcom = _common.enable_railcom_checkbox->isChecked(),
          .enable_bit_modify_on_long_address =
            _common.enable_bit_modify_on_long_address_checkbox->isChecked(),
          .key_stop_mode = static_cast<z21::CommonSettings::KeyStopMode>(
            _common.key_stop_mode_combobox->currentIndex()),
          .programming_type = static_cast<z21::CommonSettings::ProgrammingType>(
            _common.programming_type_combobox->currentIndex()),
          .enable_loconet_current_source =
            _common.enable_loconet_current_source_checkbox->isChecked(),
          .loconet_fast_clock_rate = static_cast<uint8_t>(
            _common.loconet_fast_clock_rate_slider->value()),
          .loconet_mode =
            static_cast<uint8_t>(_common.loconet_mode_combobox->currentIndex()),
          .ext_settings = commonExtFlags(),
          .purging_time =
            static_cast<uint8_t>(_common.purging_time_combobox->currentIndex()),
          .bus_settings = 0u}; // \todo
}

//
void Settings::commonSettings(z21::CommonSettings const& common_settings) {
  _common.enable_railcom_checkbox->setChecked(common_settings.enable_railcom);
  _common.enable_bit_modify_on_long_address_checkbox->setChecked(
    common_settings.enable_bit_modify_on_long_address);
  _common.key_stop_mode_combobox->setCurrentIndex(
    common_settings.key_stop_mode);
  _common.programming_type_combobox->setCurrentIndex(
    common_settings.programming_type);
  _common.enable_loconet_current_source_checkbox->setChecked(
    common_settings.enable_loconet_current_source);
  _common.loconet_fast_clock_rate_slider->setValue(
    common_settings.loconet_fast_clock_rate);
  commonExtFlags(common_settings.ext_settings);
  _common.loconet_mode_combobox->setCurrentIndex(common_settings.loconet_mode);
}

//
z21::MmDccSettings Settings::mmDccSettings() {
  return {.startup_reset_package_count = static_cast<uint8_t>(
            _mm_dcc.startup_reset_package_count_slider->value()),
          .continue_reset_packet_count = static_cast<uint8_t>(
            _mm_dcc.continue_reset_package_count_slider->value()),
          .program_package_count =
            static_cast<uint8_t>(_mm_dcc.program_package_count_slider->value()),
          .bit_verify_to_one =
            static_cast<bool>(_mm_dcc.bit_verify_to_one_slider->value()),
          .programming_ack_current = static_cast<uint8_t>(
            _mm_dcc.programming_ack_current_slider->value()),
          .flags = mmDccFlags(),
          .output_voltage =
            static_cast<uint16_t>(_mm_dcc.output_voltage_slider->value()),
          .programming_voltage =
            static_cast<uint16_t>(_mm_dcc.programming_voltage_slider->value())};
}

//
void Settings::mmDccSettings(z21::MmDccSettings const& mm_dcc_settings) {
  _mm_dcc.startup_reset_package_count_slider->setValue(
    mm_dcc_settings.startup_reset_package_count);
  _mm_dcc.continue_reset_package_count_slider->setValue(
    mm_dcc_settings.continue_reset_packet_count);
  _mm_dcc.program_package_count_slider->setValue(
    mm_dcc_settings.program_package_count);
  _mm_dcc.bit_verify_to_one_slider->setValue(mm_dcc_settings.bit_verify_to_one);
  mmDccFlags(mm_dcc_settings.flags);
  _mm_dcc.output_voltage_slider->setValue(mm_dcc_settings.output_voltage);
  _mm_dcc.programming_voltage_slider->setValue(
    mm_dcc_settings.programming_voltage);
}

//
void Settings::initCommonWidgets() {
  _common.enable_railcom_checkbox->setStatusTip("Activate RailCom");

  _common.enable_bit_modify_on_long_address_checkbox->setStatusTip(
    "Set CV29 automatically if the loco address is changed");

  _common.key_stop_mode_combobox->addItem("Track power off");
  _common.key_stop_mode_combobox->addItem("EStop");
  _common.key_stop_mode_combobox->setStatusTip("Function of the Z21 button");

  _common.programming_type_combobox->addItem("Nothing");
  _common.programming_type_combobox->addItem("Bit only");
  _common.programming_type_combobox->addItem("Byte only");
  _common.programming_type_combobox->addItem("Bit and byte");
  _common.programming_type_combobox->setStatusTip(
    "How a service mode verify is performed");

  _common.enable_loconet_current_source_checkbox->setStatusTip("?");

  _common.loconet_fast_clock_rate_slider->setMinimum(0);
  _common.loconet_fast_clock_rate_slider->setMaximum(128);
  _common.loconet_fast_clock_rate_slider->setStatusTip(
    "Specifies the rate for the model time (0=stop, 1=normal, 2=doubled, "
    "3=tripled, ...)");
  _common.loconet_fast_clock_rate_label->setText(
    QString::number(_common.loconet_fast_clock_rate_slider->value()));

  _common.loconet_mode_combobox->addItem("Off");
  _common.loconet_mode_combobox->addItem("Gateway only");
  _common.loconet_mode_combobox->addItem("Slave");
  _common.loconet_mode_combobox->addItem("Master");

  _common.ext_flags.disable_turnout_timeout_checkbox->setStatusTip(
    "Disable automatic timeout of turnout outputs");
  _common.ext_flags.disable_turnout_auto_deact_checkbox->setStatusTip(
    "Automatic turnoff of turnout output A, if output B gets activated "
    "(workaround for incompatible clients which only send output A on followed "
    "by output B on)");
  _common.ext_flags.accessory_start_group1_checkbox->setStatusTip(
    "Fixes a turnout-address incompatibility between different DCC systems");
  _common.ext_flags.rbus_as_xbus2_checkbox->setStatusTip(
    "Use R-Bus as additional X-Bus interface for throttles and other XpressNet "
    "devices");
  _common.ext_flags.invert_accessory_red_green_checkbox->setStatusTip(
    "Invert meaning of straight/branch or green/red for accessory decoders");

  _common.purging_time_combobox->addItem("Off");
  _common.purging_time_combobox->addItem("1min");
  _common.purging_time_combobox->addItem("2min");
  _common.purging_time_combobox->addItem("4min");
  _common.purging_time_combobox->addItem("8min");
  _common.purging_time_combobox->addItem("15min");
  _common.purging_time_combobox->addItem("30min");
  _common.purging_time_combobox->addItem("60min");

  _common.bus_flags.disable_emulate_rm_checkbox->setStatusTip(
    "CAN occupancy messages emulate R-Bus messages");
  _common.bus_flags.disable_emulate_ln_checkbox->setStatusTip(
    "CAN occupancy messages emulate LocoNet messages");
  _common.bus_flags.enable_emulate_lissy_checkbox->setStatusTip(
    "CAN occupancy messages emulate Lissy messages");
  _common.bus_flags.xbus_use_fallback_version_checkbox->setStatusTip(
    "Use X-Bus protocol version V3.6 instead of V4.0");
  _common.bus_flags.xbus_forwarding_rbus_checkbox->setStatusTip(
    "Forward R-Bus messages as XpressNet feedback broadcasts to X-Bus devices "
    "(overrides turnout numbers from 257-336 on X-Bus)");
  _common.bus_flags.bus_only_accessories_checkbox->setStatusTip(
    "Only pass accessory commands on the bus and lan, but do not output them "
    "on the track");

  //
  connect(_common.loconet_fast_clock_rate_slider,
          &QSlider::valueChanged,
          [this](int value) {
            _common.loconet_fast_clock_rate_label->setText(
              value & 0b1000'0000 ? QString{"off"} : QString::number(value));
          });

  //
  Config const config;
  if (auto const value{config.value("settings_enable_railcom")};
      value.isValid())
    _common.enable_railcom_checkbox->setChecked(value.toBool());
  else _common.enable_railcom_checkbox->setChecked(true);
  if (auto const value{
        config.value("settings_enable_bit_modify_on_long_address")};
      value.isValid())
    _common.enable_bit_modify_on_long_address_checkbox->setChecked(
      value.toBool());
  if (auto const value{config.value("settings_key_stop_mode")}; value.isValid())
    _common.key_stop_mode_combobox->setCurrentIndex(value.toInt());
  if (auto const value{config.value("settings_programming_type")};
      value.isValid())
    _common.programming_type_combobox->setCurrentIndex(value.toInt());
  else _common.programming_type_combobox->setCurrentIndex(3);
  if (auto const value{config.value("settings_enable_loconet_current_source")};
      value.isValid())
    _common.enable_loconet_current_source_checkbox->setChecked(value.toBool());
  if (auto const value{config.value("settings_loconet_fast_clock_rate")};
      value.isValid())
    _common.loconet_fast_clock_rate_slider->setValue(value.toInt());
  if (auto const value{config.value("settings_loconet_mode")}; value.isValid())
    _common.loconet_mode_combobox->setCurrentIndex(value.toInt());
  else _common.loconet_mode_combobox->setCurrentIndex(3);
  if (auto const value{config.value("settings_ext_flags")}; value.isValid())
    commonExtFlags(static_cast<z21::CommonSettings::ExtFlags>(value.toUInt()));
  if (auto const value{config.value("settings_purging_time")}; value.isValid())
    _common.purging_time_combobox->setCurrentIndex(value.toInt());
  if (auto const value{config.value("settings_bus_flags")}; value.isValid())
    commonBusFlags(static_cast<uint8_t>(value.toUInt()));
}

//
void Settings::initMmDccWidgets() {
  _mm_dcc.startup_reset_package_count_slider->setMinimum(25);
  _mm_dcc.startup_reset_package_count_slider->setMaximum(255);
  _mm_dcc.startup_reset_package_count_slider->setStatusTip(
    "Number of reset packets at the start of the service mode programming "
    "sequence");
  _mm_dcc.startup_reset_package_count_label->setText(
    QString::number(_mm_dcc.startup_reset_package_count_slider->value()));

  _mm_dcc.continue_reset_package_count_slider->setMinimum(6);
  _mm_dcc.continue_reset_package_count_slider->setMaximum(64);
  _mm_dcc.continue_reset_package_count_slider->setStatusTip(
    "Number of reset packets when continuing the service mode programming "
    "sequence");
  _mm_dcc.continue_reset_package_count_label->setText(
    QString::number(_mm_dcc.continue_reset_package_count_slider->value()));

  _mm_dcc.program_package_count_slider->setMinimum(7);
  _mm_dcc.program_package_count_slider->setMaximum(64);
  _mm_dcc.program_package_count_slider->setStatusTip(
    "Number of programming packets in the service mode programming sequence");
  _mm_dcc.program_package_count_label->setText(
    QString::number(_mm_dcc.program_package_count_slider->value()));

  _mm_dcc.bit_verify_to_one_slider->setMinimum(0);
  _mm_dcc.bit_verify_to_one_slider->setMaximum(1);
  _mm_dcc.bit_verify_to_one_slider->setStatusTip(
    "Comparing bits to either 0 or 1 during a service mode verify");
  _mm_dcc.bit_verify_to_one_label->setText(
    QString::number(_mm_dcc.bit_verify_to_one_slider->value()));

  _mm_dcc.programming_ack_current_slider->setMinimum(10);
  _mm_dcc.programming_ack_current_slider->setMaximum(250);
  _mm_dcc.programming_ack_current_slider->setStatusTip(
    "Programming ack current [mA]");
  _mm_dcc.programming_ack_current_label->setText(
    QString::number(_mm_dcc.programming_ack_current_slider->value()));

  _mm_dcc.flags.fmt_combobox->addItem("DCC and MM");
  _mm_dcc.flags.fmt_combobox->addItem("DCC only");
  _mm_dcc.flags.fmt_combobox->addItem("MM only");
  _mm_dcc.flags.fmt_combobox->setStatusTip("Output format on the track");

  _mm_dcc.flags.repeat_hfx_checkbox->setStatusTip(
    "Periodically refresh loco high functions (>F13)");

  _mm_dcc.flags.short_combobox->addItem("1 to 99");
  _mm_dcc.flags.short_combobox->addItem("1 to 127");
  _mm_dcc.flags.short_combobox->setStatusTip(
    "Configure the range for short DCC loco addresses");

  _mm_dcc.output_voltage_slider->setMinimum(11000);
  _mm_dcc.output_voltage_slider->setMaximum(23000);
  _mm_dcc.output_voltage_label->setText(
    QString::number(_mm_dcc.output_voltage_slider->value()));

  _mm_dcc.programming_voltage_slider->setMinimum(11000);
  _mm_dcc.programming_voltage_slider->setMaximum(23000);
  _mm_dcc.programming_voltage_label->setText(
    QString::number(_mm_dcc.programming_voltage_slider->value()));

  //
  connect(_mm_dcc.startup_reset_package_count_slider,
          &QSlider::valueChanged,
          [this](int value) {
            _mm_dcc.startup_reset_package_count_label->setText(
              QString::number(value));
          });
  connect(_mm_dcc.continue_reset_package_count_slider,
          &QSlider::valueChanged,
          [this](int value) {
            _mm_dcc.continue_reset_package_count_label->setText(
              QString::number(value));
          });
  connect(_mm_dcc.program_package_count_slider,
          &QSlider::valueChanged,
          [this](int value) {
            _mm_dcc.program_package_count_label->setText(
              QString::number(value));
          });
  connect(_mm_dcc.bit_verify_to_one_slider,
          &QSlider::valueChanged,
          [this](int value) {
            _mm_dcc.bit_verify_to_one_label->setText(QString::number(value));
          });
  connect(_mm_dcc.programming_ack_current_slider,
          &QSlider::valueChanged,
          [this](int value) {
            _mm_dcc.programming_ack_current_label->setText(
              QString::number(value));
          });
  connect(
    _mm_dcc.output_voltage_slider, &QSlider::valueChanged, [this](int value) {
      _mm_dcc.output_voltage_label->setText(QString::number(value));
    });
  connect(_mm_dcc.programming_voltage_slider,
          &QSlider::valueChanged,
          [this](int value) {
            _mm_dcc.programming_voltage_label->setText(QString::number(value));
          });

  //
  Config const config;
  if (auto const value{config.value("settings_startup_reset_package_count")};
      value.isValid())
    _mm_dcc.startup_reset_package_count_slider->setValue(value.toInt());
  if (auto const value{config.value("settings_continue_reset_package_count")};
      value.isValid())
    _mm_dcc.continue_reset_package_count_slider->setValue(value.toInt());
  if (auto const value{config.value("settings_program_package_count")};
      value.isValid())
    _mm_dcc.program_package_count_slider->setValue(value.toInt());
  if (auto const value{config.value("settings_bit_verify_to_one")};
      value.isValid())
    _mm_dcc.bit_verify_to_one_slider->setValue(value.toInt());
  else _mm_dcc.bit_verify_to_one_slider->setValue(1);
  if (auto const value{config.value("settings_programming_ack_current")};
      value.isValid())
    _mm_dcc.programming_ack_current_slider->setValue(value.toInt());
  else _mm_dcc.programming_ack_current_slider->setValue(50);
  if (auto const value{config.value("settings_mm_dcc_flags")}; value.isValid())
    mmDccFlags(static_cast<z21::MmDccSettings::Flags>(value.toUInt()));
  if (auto const value{config.value("settings_output_voltage")};
      value.isValid())
    _mm_dcc.output_voltage_slider->setValue(value.toInt());
  if (auto const value{config.value("settings_programming_voltage")};
      value.isValid())
    _mm_dcc.programming_voltage_slider->setValue(value.toInt());
}

//
void Settings::commonExtFlags(z21::CommonSettings::ExtFlags flags) {
  //
  _common.ext_flags.disable_turnout_timeout_checkbox->setChecked(
    static_cast<bool>(flags &
                      z21::CommonSettings::ExtFlags::TurnoutTimeoutDisable));

  //
  _common.ext_flags.disable_turnout_auto_deact_checkbox->setChecked(
    static_cast<bool>(flags &
                      z21::CommonSettings::ExtFlags::TurnoutAutoDeactDisable));

  //
  _common.ext_flags.accessory_start_group1_checkbox->setChecked(
    static_cast<bool>(flags &
                      z21::CommonSettings::ExtFlags::AccessoryStartGroup1));

  //
  _common.ext_flags.rbus_as_xbus2_checkbox->setChecked(
    static_cast<bool>(flags & z21::CommonSettings::ExtFlags::RBusAsXBus2));

  //
  _common.ext_flags.invert_accessory_red_green_checkbox->setChecked(
    static_cast<bool>(flags &
                      z21::CommonSettings::ExtFlags::AccessoryInvRedGreen));
}

z21::CommonSettings::ExtFlags Settings::commonExtFlags() const {
  uint8_t retval{};

  //
  if (_common.ext_flags.disable_turnout_timeout_checkbox->isChecked())
    retval |= z21::CommonSettings::ExtFlags::TurnoutTimeoutDisable;

  //
  if (_common.ext_flags.disable_turnout_auto_deact_checkbox->isChecked())
    retval |= z21::CommonSettings::ExtFlags::TurnoutAutoDeactDisable;

  //
  if (_common.ext_flags.accessory_start_group1_checkbox->isChecked())
    retval |= z21::CommonSettings::ExtFlags::AccessoryStartGroup1;

  //
  if (_common.ext_flags.rbus_as_xbus2_checkbox->isChecked())
    retval |= z21::CommonSettings::ExtFlags::RBusAsXBus2;

  //
  if (_common.ext_flags.invert_accessory_red_green_checkbox->isChecked())
    retval |= z21::CommonSettings::ExtFlags::AccessoryInvRedGreen;

  return static_cast<z21::CommonSettings::ExtFlags>(retval);
}

//
void Settings::commonBusFlags(uint8_t flags) {
  _common.bus_flags.disable_emulate_rm_checkbox->setChecked(
    static_cast<bool>(flags & 0x01u));
  _common.bus_flags.disable_emulate_ln_checkbox->setChecked(
    static_cast<bool>(flags & 0x02u));
  _common.bus_flags.enable_emulate_lissy_checkbox->setChecked(
    static_cast<bool>(flags & 0x04u));
  _common.bus_flags.xbus_use_fallback_version_checkbox->setChecked(
    static_cast<bool>(flags & 0x10u));
  _common.bus_flags.xbus_forwarding_rbus_checkbox->setChecked(
    static_cast<bool>(flags & 0x20u));
  _common.bus_flags.bus_only_accessories_checkbox->setChecked(
    static_cast<bool>(flags & 0x80u));
}

//
uint8_t Settings::commonBusFlags() const {
  uint8_t retval{};
  if (_common.bus_flags.disable_emulate_rm_checkbox->isChecked())
    retval |= 0x01u;
  if (_common.bus_flags.disable_emulate_ln_checkbox->isChecked())
    retval |= 0x02u;
  if (_common.bus_flags.enable_emulate_lissy_checkbox->isChecked())
    retval |= 0x04u;
  if (_common.bus_flags.xbus_use_fallback_version_checkbox->isChecked())
    retval |= 0x10u;
  if (_common.bus_flags.xbus_forwarding_rbus_checkbox->isChecked())
    retval |= 0x20u;
  if (_common.bus_flags.bus_only_accessories_checkbox->isChecked())
    retval |= 0x80u;
  return retval;
}

//
void Settings::mmDccFlags(z21::MmDccSettings::Flags flags) {
  //
  if (auto const fmt{flags & (z21::MmDccSettings::Flags::DccOnly |
                              z21::MmDccSettings::Flags::MmOnly)};
      fmt == z21::MmDccSettings::Flags::DccMmMixed)
    _mm_dcc.flags.fmt_combobox->setCurrentIndex(0);
  else if (fmt == z21::MmDccSettings::Flags::DccOnly)
    _mm_dcc.flags.fmt_combobox->setCurrentIndex(1);
  else if (fmt == z21::MmDccSettings::Flags::MmOnly)
    _mm_dcc.flags.fmt_combobox->setCurrentIndex(2);

  //
  _mm_dcc.flags.repeat_hfx_checkbox->setChecked(
    static_cast<bool>(flags & z21::MmDccSettings::Flags::RepeatHfx));

  //
  _mm_dcc.flags.short_combobox->setCurrentIndex(
    static_cast<bool>(flags & z21::MmDccSettings::Flags::DccShort127));
}

//
z21::MmDccSettings::Flags Settings::mmDccFlags() const {
  uint8_t retval{};

  //
  if (_mm_dcc.flags.fmt_combobox->currentIndex() == 1)
    retval |= z21::MmDccSettings::Flags::DccOnly;
  else if (_mm_dcc.flags.fmt_combobox->currentIndex() == 2)
    retval |= z21::MmDccSettings::Flags::MmOnly;

  //
  if (_mm_dcc.flags.repeat_hfx_checkbox->isChecked())
    retval |= z21::MmDccSettings::Flags::RepeatHfx;

  //
  if (_mm_dcc.flags.short_combobox->currentIndex() == 1)
    retval |= z21::MmDccSettings::Flags::DccShort127;

  return static_cast<z21::MmDccSettings::Flags>(retval);
}
