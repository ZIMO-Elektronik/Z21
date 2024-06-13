#include "main_window.hpp"
#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QScreen>
#include <QStatusBar>
#include <QToolBar>
#include "led.hpp"
#include "server.hpp"
#include "settings.hpp"

// Add menu and toolbar
MainWindow::MainWindow() {
  // Initial size
  setMinimumSize(1200, 900);

  QToolBar* toolbar{addToolBar("")};

  // Toolbar should be fixed
  toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
  toolbar->setFloatable(false);
  toolbar->setMovable(false);

  //
  QMenu* file_menu{menuBar()->addMenu("&File")};
  QMenu* about_menu{menuBar()->addMenu("&About")};

  //
  QIcon const exit_icon{":/dark-green/window_close.svg"};
  auto exit_act{new QAction{exit_icon, "&Exit", this}};
  exit_act->setShortcuts(QKeySequence::Quit);
  exit_act->setStatusTip("Exit the application");
  connect(exit_act, &QAction::triggered, this, &QWidget::close);
  file_menu->addAction(exit_act);
  toolbar->addAction(exit_act);

  //
  QIcon const about_icon{":/dark-green/dialog_help.svg"};
  auto about_act{new QAction{about_icon, "&About", this}};
  about_act->setStatusTip("Show the application's About box");
  connect(about_act, &QAction::triggered, this, &MainWindow::about);
  about_menu->addAction(about_act);
  toolbar->addAction(about_act);

  //
  QIcon const clear_icon{":/dark-green/trash.svg"};
  auto clear_act{new QAction{clear_icon, "&Clear data", this}};
  clear_act->setStatusTip("Clear stored data");
  connect(clear_act, &QAction::triggered, _server, &Server::clearData);
  toolbar->addAction(clear_act);

  // Add status bar
  auto status_bar{statusBar()};
  auto led{new Led{this}};
  status_bar->addPermanentWidget(led);

  // Set central widget
  setCentralWidget(_server);

  //
  connect(_server, &Server::ledStatus, led, &Led::setStatus);

  //
  restoreGeometryState();
}

//
MainWindow::~MainWindow() {
  Settings settings;
  settings.setValue("main_window_geometry", saveGeometry());
  settings.setValue("main_window_state", saveState());
}

// Show about message box
void MainWindow::about() {
  QString ip_addrs_str{};
  for (QHostAddress const& address : QNetworkInterface::allAddresses())
    if (!address.isLoopback() &&
        address.protocol() == QAbstractSocket::IPv4Protocol &&
        address != QHostAddress(QHostAddress::LocalHost))
      ip_addrs_str += address.toString() + "<br>";

  QMessageBox about;
  about.setWindowTitle("About " + QCoreApplication::applicationName());
  about.setTextFormat(Qt::RichText);
  about.setText("<b>Possible IP addresses</b><br>" + ip_addrs_str);
  about.setDetailedText("@Version\n" + QCoreApplication::applicationVersion() +
                        "\n\n" +       //
                        "@GitHub\n" +  //
                        "github.com/ZIMO-Elektronik/Z21");
  about.setStandardButtons(QMessageBox::Ok);
  about.setIconPixmap(QPixmap{":/images/logo.png"});
  about.show();
  about.exec();
}

//
void MainWindow::restoreGeometryState() {
  Settings const settings;
  if (auto const geometry{settings.value("main_window_geometry")};
      geometry.isValid())
    restoreGeometry(geometry.toByteArray());
  if (auto const state{settings.value("main_window_state")}; state.isValid())
    restoreState(state.toByteArray());
}
