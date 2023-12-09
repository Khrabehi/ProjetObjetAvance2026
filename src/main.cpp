
#include <gui/MainWindow.hpp>

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Q_INIT_RESOURCE(resource);

  NomCool::gui::MainWindow fenetre;
  fenetre.show();

  return app.exec();
}
