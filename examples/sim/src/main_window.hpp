#pragma once

#include <QMainWindow>
#include "server.hpp"

// Main window
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

private slots:
  void about();

private:
  void restoreGeometryState();

  Server* _server{new Server{this}};
};
