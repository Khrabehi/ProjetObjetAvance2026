
#include <gui/MainWindow.hpp>
#include "services/QuizEngine.hpp"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  Q_INIT_RESOURCE(resource);

  // Instancier le service de quiz 
  NomCool::services::QuizEngine quizEngine;

  // Injecter le service de quiz dans la fenêtre
  NomCool::gui::MainWindow fenetre(&quizEngine);
  fenetre.show();

  return app.exec();
}
