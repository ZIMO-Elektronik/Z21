#include "log.hpp"
#include <QClipboard>
#include <QGridLayout>
#include <QGuiApplication>
#include <QPushButton>

//
Log::Log(QWidget* parent) : QWidget{parent} {
  // Clear button
  QPixmap const clear_pixmap{":/dark-green/clear_text.svg"};
  auto clear_button{new QPushButton};
  clear_button->setStatusTip("Clear");
  clear_button->setToolTip("Clear");
  clear_button->setIconSize(QSize{20, 20});
  clear_button->setFixedSize(QSize{20, 20});
  clear_button->setIcon(clear_pixmap);
  connect(clear_button, &QPushButton::clicked, _text_view, &QTextEdit::clear);

  // Copy button
  QPixmap const copy_pixmap{":/dark-green/file_dialog_detailed.svg"};
  auto copy_button{new QPushButton};
  copy_button->setStatusTip("Copy to clipboard");
  copy_button->setToolTip("Copy to clipboard");
  copy_button->setIconSize(QSize{20, 20});
  copy_button->setFixedSize(QSize{20, 20});
  copy_button->setIcon(copy_pixmap);
  connect(copy_button, &QPushButton::clicked, [this] {
    QClipboard* clipboard{QGuiApplication::clipboard()};
    clipboard->setText(_text_view->toPlainText());
  });

  // Line wrapping would fuck with alignment
  _text_view->setLineWrapMode(QTextEdit::NoWrap);

  // Layout
  auto layout{new QGridLayout};
  layout->setContentsMargins(11, 0, 11, 11);
  layout->addWidget(clear_button, 0, 0);
  layout->addWidget(copy_button, 0, 1);
  layout->addWidget(_text_view, 1, 0, 8, 8);

  setLayout(layout);
}

//
void Log::append(QString const& text) { _text_view->append(text); }

//
void Log::clear() { _text_view->clear(); }