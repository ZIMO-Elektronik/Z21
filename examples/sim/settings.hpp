#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QSlider>
#include <tuple>
#include <z21/z21.hpp>

//
class Settings : public QWidget {
  Q_OBJECT

public:
  explicit Settings(QWidget* parent = nullptr);
  ~Settings();

public slots:
  z21::CommonSettings commonSettings();
  void commonSettings(z21::CommonSettings const& common_settings);
  z21::MmDccSettings mmDccSettings();
  void mmDccSettings(z21::MmDccSettings const& mm_dcc_settings);

private:
  void initCommonWidgets();
  void initMmDccWidgets();

  void commonExtFlags(uint8_t flags);
  uint8_t commonExtFlags() const;

  void commonBusFlags(uint8_t flags);
  uint8_t commonBusFlags() const;

  void mmDccFlags(uint8_t flags);
  uint8_t mmDccFlags() const;

  struct {
    QCheckBox* enable_railcom_checkbox{new QCheckBox};
    QCheckBox* enable_bit_modify_on_long_address_checkbox{new QCheckBox};
    QComboBox* key_stop_mode_combobox{new QComboBox};
    QComboBox* programming_type_combobox{new QComboBox};
    QCheckBox* enable_loconet_current_source_checkbox{new QCheckBox};
    QLabel* loconet_fast_clock_rate_label{new QLabel};
    QSlider* loconet_fast_clock_rate_slider{new QSlider{Qt::Horizontal}};
    QComboBox* loconet_mode_combobox{new QComboBox};
    struct {
      QCheckBox* disable_turnout_timeout_checkbox{new QCheckBox};
      QCheckBox* disable_turnout_auto_deact_checkbox{new QCheckBox};
      QCheckBox* accessory_start_group1_checkbox{new QCheckBox};
      QCheckBox* rbus_as_xbus2_checkbox{new QCheckBox};
      QCheckBox* invert_accessory_red_green_checkbox{new QCheckBox};
    } ext_flags;
    QComboBox* purging_time_combobox{new QComboBox};
    struct {
      QCheckBox* disable_emulate_rm_checkbox{new QCheckBox};
      QCheckBox* disable_emulate_ln_checkbox{new QCheckBox};
      QCheckBox* enable_emulate_lissy_checkbox{new QCheckBox};
      QCheckBox* xbus_use_fallback_version_checkbox{new QCheckBox};
      QCheckBox* xbus_forwarding_rbus_checkbox{new QCheckBox};
      QCheckBox* bus_only_accessories_checkbox{new QCheckBox};
    } bus_flags;
  } _common;

  struct {
    QLabel* startup_reset_package_count_label{new QLabel};
    QSlider* startup_reset_package_count_slider{new QSlider{Qt::Horizontal}};
    QLabel* continue_reset_package_count_label{new QLabel};
    QSlider* continue_reset_package_count_slider{new QSlider{Qt::Horizontal}};
    QLabel* program_package_count_label{new QLabel};
    QSlider* program_package_count_slider{new QSlider{Qt::Horizontal}};
    QLabel* bit_verify_to_one_label{new QLabel};
    QSlider* bit_verify_to_one_slider{new QSlider{Qt::Horizontal}};
    QLabel* programming_ack_current_label{new QLabel};
    QSlider* programming_ack_current_slider{new QSlider{Qt::Horizontal}};
    struct {
      QComboBox* fmt_combobox{new QComboBox};
      QCheckBox* repeat_hfx_checkbox{new QCheckBox};
      QComboBox* short_combobox{new QComboBox};
    } flags;
    QLabel* output_voltage_label{new QLabel};
    QSlider* output_voltage_slider{new QSlider{Qt::Horizontal}};
    QLabel* programming_voltage_label{new QLabel};
    QSlider* programming_voltage_slider{new QSlider{Qt::Horizontal}};
  } _mm_dcc;
};