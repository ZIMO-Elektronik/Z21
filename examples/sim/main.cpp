#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include "main_window.hpp"

int main(int argc, char* argv[]) {
  QCoreApplication::setApplicationName("Z21Sim");
  QCoreApplication::setApplicationVersion(Z21SIM_VERSION);

  // Create an application instance
  QApplication app{argc, argv};

  // Initialize resources
  Q_INIT_RESOURCE(qtbreeze_stylesheets);

  // Default to monospace font
  QFontDatabase::addApplicationFont(":/fonts/DejaVuSansMono.ttf");
  QFont font{"DejaVuSansMono"};
  font.setPointSize(10);
  app.setFont(font);

  // Apply breeze stylesheet
  QFile file{":/dark-green/stylesheet.qss"};
  file.open(QFile::ReadOnly | QFile::Text);
  QTextStream stream{&file};
  app.setStyleSheet(stream.readAll());

  MainWindow w{};
  w.show();
  return app.exec();
}