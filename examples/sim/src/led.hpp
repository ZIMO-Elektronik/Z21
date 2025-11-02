#pragma once

#include <QWidget>

// LED status indicator
//
// For color codes see:
// https://www.z21.eu/en/products/z21/connections-z21#52-1222
class Led : public QWidget {
  Q_OBJECT
public:
  enum Status {
    Disconnected,
    NormalOperation,
    ProgrammingMode,
    DecoderUpdate,
    Stop,
    ShortCircuit,
  };

  explicit Led(QWidget* parent = nullptr);

public slots:
  void setStatus(Status status);

protected:
  void paintEvent(QPaintEvent* event) final;

private slots:
  void timeout();

private:
  static constexpr QSize _size{60, 10};
  QColor _color{Qt::gray};
  Status _status{};
};
