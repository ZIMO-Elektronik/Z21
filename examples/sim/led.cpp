#include "led.hpp"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>

// Create timer with 200ms base
Led::Led(QWidget* parent) : QWidget{parent} {
  setMinimumSize(80, 10 + 15);
  auto timer{new QTimer{this}};
  connect(timer, &QTimer::timeout, this, &Led::timeout);
  timer->start(200);
}

// Set status and immediately emit an update
void Led::setStatus(Status status) {
  _status = status;
  emit update();
}

// Try to replicate original Z21 LED status indicator as close as possible
void Led::paintEvent(QPaintEvent* event) {
  QPainter p{this};
  p.setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  QRectF r{0, 0, 80, 10};
  path.addRoundedRect(r, 10, 10);
  QPen pen{_color, 1};
  p.setPen(pen);
  p.fillPath(path, _color);
  p.drawPath(path);
  p.setPen(QColor("#EFF0F1"));
  p.drawText(25, 25, "STOP");
}

// Update LED color based on status
void Led::timeout() {
  switch (_status) {
    case Disconnected: _color = Qt::gray; break;
    case NormalOperation: _color = Qt::blue; break;
    case ProgrammingMode: _color = Qt::green; break;
    case DecoderUpdate:
      _color = _color == Qt::green ? Qt::gray : Qt::green;
      break;
    case Stop: _color = _color == Qt::blue ? Qt::gray : Qt::blue; break;
    case ShortCircuit: _color = _color == Qt::red ? Qt::gray : Qt::red; break;
  }

  emit update();
}
