
#include "gui/MainWindow.hpp"
#include "services/InventoryRepository.hpp"
#include "services/QuizEngine.hpp"

#include <QApplication>
#include <QDir>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(resource);

  QApplication app(argc, argv);
  app.setApplicationName("EL CALCULATOR");
  app.setOrganizationName("ElCalculator");

  // Chemin de sauvegarde
  QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir().mkpath(appDataPath); // Crée le dossier s'il n'existe pas
  QString saveFile = appDataPath + "/inventory.json";

  ElCalculator::services::QuizEngine quizEngine;
  bool loadSuccess = ElCalculator::services::InventoryRepository::loadInventory(
      quizEngine.getInventory(), saveFile);

  // Injecter le service de quiz dans la fenêtre
  ElCalculator::gui::MainWindow fenetre(&quizEngine);
  fenetre.show();

  return app.exec();
}
