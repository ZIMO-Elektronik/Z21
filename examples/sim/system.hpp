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
  void broadcastProgrammingMode();
  void broadcastTrackShortCircuit();
  void broadcastStopped();
  void broadcastSystemStateData();

private:
  using WidgetTriplet = std::
    tuple<std::vector<QLabel*>, std::vector<QLabel*>, std::vector<QSlider*>>;

  void initFailureRatesWidgets();
  void initCurrentsVoltagesTemperatureWidgets();

  WidgetTriplet _failure_rates_widgets;
  WidgetTriplet _system_state_widgets;
};