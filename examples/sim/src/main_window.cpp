#include "main_window.hpp"
#include <QApplication>
#include <QGuiApplication>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QScreen>
#include <QStatusBar>
#include <QToolBar>
#include "config.hpp"
#include "led.hpp"
#include "server.hpp"

// Create layout with menus, toolbars and tabs
MainWindow::MainWindow() {
  // Initial size that's friendly to common macOS laptop resolutions and scales
  // with the primary screen (restored size wins if saved previously).
  applyDefaultSize();

  QToolBar* toolbar{addToolBar("")};

  // Toolbar should be fixed
  toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
  toolbar->setFloatable(false);
  toolbar->setMovable(false);

  // Menus
  QMenu* file_menu{menuBar()->addMenu("&File")};
  QMenu* about_menu{menuBar()->addMenu("&About")};

  // Exit action
  QIcon const exit_icon{":/dark-green/window_close.svg"};
  auto exit_act{new QAction{exit_icon, "&Exit", this}};
  exit_act->setShortcuts(QKeySequence::Quit);
  exit_act->setStatusTip("Exit the application");
  connect(exit_act, &QAction::triggered, this, &QWidget::close);
  file_menu->addAction(exit_act);
  toolbar->addAction(exit_act);

  // About action
  QIcon const about_icon{":/dark-green/dialog_help.svg"};
  auto about_act{new QAction{about_icon, "&About", this}};
  about_act->setStatusTip("Show the application's About box");
  connect(about_act, &QAction::triggered, this, &MainWindow::about);
  about_menu->addAction(about_act);
  toolbar->addAction(about_act);

  // Clear data actions
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

  // Connect LED in status bar to server
  connect(_server, &Server::ledStatus, led, &Led::setStatus);

  // Restore geometry and state
  restoreGeometryState();
}

// Store geometry and state in config
MainWindow::~MainWindow() {
  Config config;
  config.setValue("main_window_geometry", saveGeometry());
  config.setValue("main_window_state", saveState());
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
                        "\n\n" +      //
                        "@GitHub\n" + //
                        "github.com/ZIMO-Elektronik/Z21");
  about.setStandardButtons(QMessageBox::Ok);
  about.setIconPixmap(QPixmap{":/images/logo.png"});
  about.show();
  about.exec();
}

// Restore geometry and state
void MainWindow::restoreGeometryState() {
  Config const config;
  if (auto const geometry{config.value("main_window_geometry")};
      geometry.isValid())
    restoreGeometry(geometry.toByteArray());
  if (auto const state{config.value("main_window_state")}; state.isValid())
    restoreState(state.toByteArray());
}

// Apply a sensible default size based on available screen real estate
void MainWindow::applyDefaultSize() {
  auto* screen{QGuiApplication::primaryScreen()};
  if (!screen) return;

  auto const available{screen->availableGeometry()};

  // Keep the window usable on smaller MacBooks while giving room for the tabs
  QSize const min_size{std::min(available.width(), 1280),
                       std::min(available.height(), 800)};
  setMinimumSize(min_size);

  QSize const desired{std::clamp(static_cast<int>(available.width() * 0.8),
                                 min_size.width(),
                                 available.width()),
                      std::clamp(static_cast<int>(available.height() * 0.8),
                                 min_size.height(),
                                 available.height())};

  resize(desired);
  move(available.center() - rect().center());
}
