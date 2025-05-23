#pragma once

#include <QRegularExpression>
#include <QWidget>
#include "text_view.hpp"

// Log window for client and server messages
class Log : public QWidget {
  Q_OBJECT

public:
  explicit Log(QWidget* parent = nullptr);
  ~Log();

public slots:
  void append(QString const& text);
  void clear();

private:
  QString _name{};
  TextView* _text_view{new TextView{this}};
  QRegularExpression _re_filter;
};
