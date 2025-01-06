#include "log.hpp"
#include <QClipboard>
#include <QGridLayout>
#include <QGuiApplication>
#include <QLineEdit>
#include <QPushButton>
#include "config.hpp"

//
Log::Log(QWidget* parent) : QWidget{parent} {
  static int is_log_server_instance{};
  _name = is_log_server_instance++ ? "log_server" : "log_client";

  // Clear button
  QPixmap const clear_pixmap{":/dark-green/clear_text.svg"};
  auto clear_button{new QPushButton};
  clear_button->setToolTip("Clear");
  clear_button->setStatusTip("Clear entire log");
  clear_button->setIconSize(QSize{20, 20});
  clear_button->setFixedSize(QSize{20, 20});
  clear_button->setIcon(clear_pixmap);
  connect(clear_button, &QPushButton::clicked, _text_view, &QTextEdit::clear);

  // Copy button
  QPixmap const copy_pixmap{":/dark-green/file_dialog_detailed.svg"};
  auto copy_button{new QPushButton};
  copy_button->setToolTip("Copy to clipboard");
  copy_button->setStatusTip("Copy entire log to clipboard");
  copy_button->setIconSize(QSize{20, 20});
  copy_button->setFixedSize(QSize{20, 20});
  copy_button->setIcon(copy_pixmap);
  connect(copy_button, &QPushButton::clicked, [this] {
    QClipboard* clipboard{QGuiApplication::clipboard()};
    clipboard->setText(_text_view->toPlainText());
  });

  // Filter
  auto line_edit{new QLineEdit{this}};
  line_edit->setToolTip("Filter");
  line_edit->setStatusTip("Filter using regular expression");

  // Line wrapping would fuck with alignment
  _text_view->setLineWrapMode(QTextEdit::NoWrap);

  // Layout
  auto layout{new QGridLayout};
  layout->setContentsMargins(11, 0, 11, 11);
  layout->addWidget(clear_button, 0, 0);
  layout->addWidget(copy_button, 0, 1);
  layout->addWidget(line_edit, 0, 2);
  layout->addWidget(_text_view, 1, 0, 8, 8);

  setLayout(layout);

  //
  connect(line_edit, &QLineEdit::textChanged, [this](QString const& text) {
    _re_filter = QRegularExpression{text};
  });

  //
  Config const config;
  if (auto const value{config.value(_name + "_re_filter")}; value.isValid())
    line_edit->setText(value.toString());
}

//
Log::~Log() {
  Config config;
  config.setValue(_name + "_re_filter", _re_filter.pattern());
}

//
void Log::append(QString const& text) {
  if (_re_filter.match(text).hasMatch()) _text_view->append(text);
}

//
void Log::clear() { _text_view->clear(); }