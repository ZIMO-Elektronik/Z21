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
    QCheckBox* enable_railcom_checkbox;
    QCheckBox* enable_bit_modify_on_long_address_checkbox;
    QComboBox* key_stop_mode_combobox;
    QComboBox* programming_type_combobox;
    QCheckBox* enable_loconet_current_source_checkbox;
    QLabel* loconet_fast_clock_rate_label;
    QSlider* loconet_fast_clock_rate_slider;
    QComboBox* loconet_mode_combobox;
    struct {
      QCheckBox* disable_turnout_timeout_checkbox;
      QCheckBox* disable_turnout_auto_deact_checkbox;
      QCheckBox* accessory_start_group1_checkbox;
      QCheckBox* rbus_as_xbus2_checkbox;
      QCheckBox* invert_accessory_red_green_checkbox;
    } ext_flags;
    QComboBox* purging_time_combobox;
    struct {
      QCheckBox* emulate_rm_checkbox;
      QCheckBox* emulate_ln_checkbox;
      QCheckBox* emulate_lissy_checkbox;
      QCheckBox* xbus_use_fallback_version_checkbox;
      QCheckBox* xbus_forwarding_rbus_checkbox;
      QCheckBox* bus_only_accessories_checkbox;
    } bus_flags;
  } _common{new QCheckBox{this},
            new QCheckBox{this},
            new QComboBox{this},
            new QComboBox{this},
            new QCheckBox{this},
            new QLabel{this},
            new QSlider{Qt::Horizontal, this},
            new QComboBox{this},
            {new QCheckBox{this},
             new QCheckBox{this},
             new QCheckBox{this},
             new QCheckBox{this},
             new QCheckBox{this}},
            new QComboBox{this},
            {new QCheckBox{this},
             new QCheckBox{this},
             new QCheckBox{this},
             new QCheckBox{this},
             new QCheckBox{this},
             new QCheckBox{this}}};

  struct {
    QLabel* startup_reset_package_count_label;
    QSlider* startup_reset_package_count_slider;
    QLabel* continue_reset_package_count_label;
    QSlider* continue_reset_package_count_slider;
    QLabel* program_package_count_label;
    QSlider* program_package_count_slider;
    QLabel* bit_verify_to_one_label;
    QSlider* bit_verify_to_one_slider;
    QLabel* programming_ack_current_label;
    QSlider* programming_ack_current_slider;
    struct {
      QComboBox* fmt_combobox;
      QCheckBox* repeat_hfx_checkbox;
      QComboBox* short_combobox;
    } flags;
    QLabel* output_voltage_label;
    QSlider* output_voltage_slider;
    QLabel* programming_voltage_label;
    QSlider* programming_voltage_slider;
  } _mm_dcc{new QLabel{this},
            new QSlider{Qt::Horizontal, this},
            new QLabel{this},
            new QSlider{Qt::Horizontal, this},
            new QLabel{this},
            new QSlider{Qt::Horizontal, this},
            new QLabel{this},
            new QSlider{Qt::Horizontal, this},
            new QLabel{this},
            new QSlider{Qt::Horizontal, this},
            {
              new QComboBox{this},
              new QCheckBox{this},
              new QComboBox{this},
            },
            new QLabel{this},
            new QSlider{Qt::Horizontal, this},
            new QLabel{this},
            new QSlider{Qt::Horizontal, this}};
};