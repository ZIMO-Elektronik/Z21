#pragma once

#include <QLabel>
#include <QSlider>
#include <tuple>

//
class System : public QWidget {
  Q_OBJECT

public:
  explicit System(QWidget* parent = nullptr);
  ~System();

  bool programmingShortCircuitFailure() const;
  bool programmingFailure() const;
  int16_t mainCurrent() const;
  int16_t progCurrent() const;
  int16_t filteredMainCurrent() const;
  int16_t temperature() const;
  uint16_t supplyVoltage() const;
  uint16_t vccVoltage() const;

signals:
  void broadcastTrackPowerOff();
  void broadcastTrackPowerOn();
  void broadcastTrackShortCircuit();
  void broadcastStopped();
  void broadcastSystemStateData();

private:
  using SlidersTriplet = std::
    tuple<std::vector<QLabel*>, std::vector<QLabel*>, std::vector<QSlider*>>;

  void initFailureRatesSliders();
  void initCurrentsVoltagesTemperatureSliders();

  SlidersTriplet _failure_rates_sliders;
  SlidersTriplet _system_state_sliders;
};