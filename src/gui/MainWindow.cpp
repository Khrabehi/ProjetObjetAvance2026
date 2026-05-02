
#include "MainWindow.hpp"

#include "InventoryWidget.hpp"

#include "services/QuizEngine.hpp"
#include "services/InventoryRepository.hpp"

#include <gui/Interrogation.hpp>

#include <data/Interrogation.hpp>

#include <stdexcept>

#include <QHBoxLayout>
#include <QPushButton>
namespace ElCalculator::gui
{

  MainWindow::MainWindow(services::QuizEngine *quizEngine)
      : mQuizEngine(quizEngine)
  {

    if (!mQuizEngine)
    {
      throw std::invalid_argument("Erreur : MainWindow a reçu un QuizEngine nul.");
    }

    mMainLayout = new QGridLayout();
    mMainLayout->setContentsMargins(16, 16, 16, 16);
    mMainLayout->setHorizontalSpacing(12);
    mMainLayout->setVerticalSpacing(12);

    auto *inventoryPanel = new InventoryWidget();
    inventoryPanel->hide();

    connect(mQuizEngine, &services::QuizEngine::inventoryUpdated,
            inventoryPanel, &InventoryWidget::updateInventory);

    // Init label pour la difficulté
    mDifficultyLabel = new QLabel("Niveau : Facile", this);
    mDifficultyLabel->setAlignment(Qt::AlignCenter);
    mDifficultyLabel->setStyleSheet("font-weight: bold; color: #2980b9; font-size: 14px;");
    mDifficultyLabel->hide(); // On le cache tant que le quiz n'a pas démarré

    auto *topBar = new QHBoxLayout();
    topBar->setContentsMargins(0, 0, 0, 0);
    topBar->setSpacing(12);
    topBar->addWidget(inventoryPanel, 1);
    topBar->addWidget(mDifficultyLabel, 0, Qt::AlignCenter);

    connect(mQuizEngine, &services::QuizEngine::difficultyChanged,
            this, [this](data::Difficulty newDiff)
            {
                QString levelText = "Niveau : ";
                switch(newDiff) {
                    case data::Difficulty::Easy: levelText += "Facile"; break;
                    case data::Difficulty::Medium: levelText += "Moyen"; break;
                    case data::Difficulty::Hard: levelText += "Difficile"; break;
                    case data::Difficulty::Expert: levelText += "Expert"; break;
                }
                mDifficultyLabel->setText(levelText); });

    // Connexion des items avec le moteur de quiz
    connect(inventoryPanel, &InventoryWidget::itemUsed, this, [this](data::ItemType type)
            {
      if (mQuizEngine->useItem(type)) {
          
          switch (type) {
              case data::ItemType::Skip:
                  setInterrogation(mQuizEngine->genererProchaineInterrogation());
                  break;
                  
              case data::ItemType::Hint:
                  if (mInterrogation) mInterrogation->displayHint("Un nombre pair..."); 
                  break;
                  
              case data::ItemType::Solve:
                  setPreviousResult({data::Result::Status::Success, "Réponse révélée par l'item !"});
                  setInterrogation(mQuizEngine->genererProchaineInterrogation());
                  break;
                  
              case data::ItemType::DeleteAnswer:
                  if (mInterrogation) mInterrogation->hideWrongAnswer();
                  break;
          }
      } });

    auto *btnDemarrer = new QPushButton("Démarrer le Quiz");
    connect(btnDemarrer, &QPushButton::clicked, this,
            [this, btnDemarrer, inventoryPanel]
            {
              setInterrogation(mQuizEngine->genererProchaineInterrogation());
              btnDemarrer->hide(); // On cache le bouton une fois le quiz lancé
              inventoryPanel->show();
              mDifficultyLabel->show();
            });
    topBar->addWidget(btnDemarrer, 0, Qt::AlignRight);

    connect(this, &MainWindow::responseSelected, this,
            [this](data::Response response)
            {
              data::Result result = mQuizEngine->traiterReponse(response);
              setPreviousResult(result);

              setInterrogation(mQuizEngine->genererProchaineInterrogation());
            });

    // Quit button (dans la barre du haut)
    auto *quitButton = new QPushButton("Quit");
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::close);
    topBar->addWidget(quitButton, 0, Qt::AlignRight);

    mMainLayout->addLayout(topBar, 0, 0);

    // This row is reserved for the previous result widget
    // The next one should be added at the mPreviousResultPosition.first + 1
    mPreviousResultPosition = {mMainLayout->rowCount(), 0};

    // This row is reserved for the interrogation widget
    // The next one should be added at the mInterrogationPosition.first + 1
    mInterrogationPosition = {mPreviousResultPosition.first + 1, 0};

    auto *centralWidget = new QWidget();
    centralWidget->setLayout(mMainLayout);
    setCentralWidget(centralWidget);

    inventoryPanel->updateInventory(&mQuizEngine->getInventory());
  }

  void MainWindow::setInterrogation(const data::Interrogation &interrogation)
  {
    if (mInterrogation)
    {
      mMainLayout->removeWidget(mInterrogation);
      delete mInterrogation;
    }
    mInterrogation = new Interrogation(interrogation);
    connect(mInterrogation, &Interrogation::responseSelected, this,
            &MainWindow::responseSelected);
    mMainLayout->addWidget(mInterrogation, mInterrogationPosition.first,
                           mInterrogationPosition.second);
  }

  void MainWindow::setPreviousResult(const data::Result &result)
  {
    if (mPreviousResult)
    {
      mMainLayout->removeWidget(mPreviousResult);
      delete mPreviousResult;
    }
    mPreviousResult = new PreviousResult(result);
    mMainLayout->addWidget(mPreviousResult, mPreviousResultPosition.first,
                           mPreviousResultPosition.second);
  }

  void ElCalculator::gui::MainWindow::closeEvent(QCloseEvent *event)
  {
    QString saveFile = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/inventory.json";

    if (mQuizEngine)
    {
      if (!services::InventoryRepository::saveInventory(
              mQuizEngine->getInventory(), saveFile))
      {
        qWarning() << "Inventaire non sauvegarde:" << saveFile;
      }
    }

    event->accept();
  }

} // namespace ElCalculator::gui
