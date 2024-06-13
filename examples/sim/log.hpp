#pragma once

#include <QWidget>
#include "text_view.hpp"

//
class Log : public QWidget {
  Q_OBJECT

public:
  explicit Log(QWidget* parent = nullptr);

public slots:
  void append(QString const& text);
  void clear();

private:
  TextView* _text_view{new TextView{this}};
};