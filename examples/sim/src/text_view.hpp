#pragma once

#include <QTextEdit>

// Read-only QTextEdit with pasting and typing disabled
class TextView : public QTextEdit {
  Q_OBJECT

public:
  explicit TextView(QWidget* parent = nullptr);

private:
  void contextMenuEvent(QContextMenuEvent* event) final;
  void insertFromMimeData(QMimeData const*) final;
  void keyPressEvent(QKeyEvent*) final;
};
