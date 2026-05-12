
#include "gui/MainWindow.hpp"
#include "services/InventoryRepository.hpp"
#include "services/QuizEngine.hpp"
#include "services/GameHistoryRepository.hpp"
#include "data/Item.hpp"

#include <QApplication>
#include <QDir>
#include <QDebug>
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
  QString invFile = appDataPath + "/inventory.json";
  QString historyFile = appDataPath + "/history.json";

  ElCalculator::services::QuizEngine quizEngine;
  if (!ElCalculator::services::InventoryRepository::loadInventory(
          quizEngine.getInventory(), invFile))
  {
    qWarning() << "Inventaire non charge:" << invFile;
  }

  ElCalculator::services::InventoryRepository::loadInventory(quizEngine.getInventory(), invFile);
  // Historique des parties
  std::vector<ElCalculator::data::GameSession> loadedHistory =
      ElCalculator::services::GameHistoryRepository::loadHistory(historyFile);
  quizEngine.setHistory(loadedHistory);
  
  quizEngine.getInventory().addItem(ElCalculator::data::ItemType::DeleteAnswer, 99);

  // Injecter le service de quiz dans la fenêtre
  ElCalculator::gui::MainWindow fenetre(&quizEngine);
  fenetre.show();

  return app.exec();
}
