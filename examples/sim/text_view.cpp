#include "text_view.hpp"
#include <QContextMenuEvent>
#include <QMenu>

TextView::TextView(QWidget* parent) : QTextEdit{parent} {}

// Use standard context menu but delete a bunch of options
void TextView::contextMenuEvent(QContextMenuEvent* event) {
  auto menu{createStandardContextMenu()};
  auto const actions{menu->actions()};
  menu->removeAction(actions[0uz]); // Undo
  menu->removeAction(actions[1uz]); // Redo
  menu->removeAction(actions[2uz]); // Separator
  menu->removeAction(actions[5uz]); // Paste
  menu->exec(event->globalPos());
  delete menu;
}

// Disable pasting
void TextView::insertFromMimeData(QMimeData const*) {}

// Disable typing
void TextView::keyPressEvent(QKeyEvent*) {}
