
#include "MainWindow.hpp"

#include "InventoryWidget.hpp"
#include "ScorePanel.hpp"

#include "services/GameHistoryRepository.hpp"

#include "services/QuizEngine.hpp"
#include "services/InventoryRepository.hpp"

#include <gui/Interrogation.hpp>

#include <data/Interrogation.hpp>

#include <stdexcept>

#include <QHBoxLayout>
#include <QPushButton>
#include <random>
#include <cctype>
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

    mInventoryPanel = new InventoryWidget();
    mInventoryPanel->hide();

    mScorePanel = new ScorePanel(mQuizEngine, this);
    mMainLayout->addWidget(mScorePanel, 0, 2, 4, 1);
    mScorePanel->updateScores();

    connect(mQuizEngine, &services::QuizEngine::sessionEnded, mScorePanel, &ScorePanel::updateScores);

    connect(mQuizEngine, &services::QuizEngine::inventoryUpdated,
            mInventoryPanel, &InventoryWidget::updateInventory);

    // Init label pour la difficulté
    mDifficultyLabel = new QLabel("Niveau : Facile", this);
    mDifficultyLabel->setAlignment(Qt::AlignCenter);
    mDifficultyLabel->setStyleSheet("font-weight: bold; color: #2980b9; font-size: 14px;");
    mDifficultyLabel->hide(); // On le cache tant que le quiz n'a pas démarré

    mLivesLabel = new QLabel("❤️ Vies : -", this);
    mLivesLabel->setAlignment(Qt::AlignCenter);
    mLivesLabel->setStyleSheet("font-weight: bold; color: #e74c3c; font-size: 14px;");
    mLivesLabel->hide(); // Caché avant le début du quiz

    auto *topBar = new QHBoxLayout();
    topBar->setContentsMargins(0, 0, 0, 0);
    topBar->setSpacing(12);
    topBar->addWidget(mInventoryPanel, 1);
    topBar->addWidget(mDifficultyLabel, 0, Qt::AlignCenter);
    topBar->addWidget(mLivesLabel, 0, Qt::AlignCenter);

    connect(mQuizEngine, &services::QuizEngine::livesChanged, this, [this](int lives)
            { mLivesLabel->setText(QString("❤️ Vies : %1").arg(lives)); });

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
    connect(mInventoryPanel, &InventoryWidget::itemUsed, this, [this](data::ItemType type)
            {
      if (type == data::ItemType::DeleteAnswer) {
        if (mInterrogation) {
          bool hidden = mInterrogation->hideWrongAnswer();
          if (hidden) {
            mQuizEngine->useItem(type);
          }
          mInventoryPanel->setItemUsable(data::ItemType::DeleteAnswer, mInterrogation->canHideWrongAnswer());
        }
        return;
      }

      if (mQuizEngine->useItem(type)) {
        switch (type) {
          case data::ItemType::Skip:
            setInterrogation(mQuizEngine->genererProchaineInterrogation());
            break;

            case data::ItemType::Hint:
              if (mInterrogation) {
                const std::string ans = mQuizEngine->getDerniereBonneReponse();
                QString hintText;
                // Construction de l'indice : affiche un chiffre présent dans la réponse.
                if (!ans.empty()) {
                  std::vector<char> digits;
                  for (char c : ans) {
                    if (std::isdigit(static_cast<unsigned char>(c))) digits.push_back(c);
                  }
                  if (!digits.empty()) {
                    static std::random_device rd;
                    static std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dis(0, (int)digits.size() - 1);
                    char chosen = digits[dis(gen)];
                    hintText = QString("La réponse contient le chiffre %1").arg(QChar(chosen));
                  } else {
                    hintText = QString::fromStdString("Indice : commence par " + ans.substr(0,1));
                  }
                } else {
                  hintText = "Indice indisponible";
                }
                mInterrogation->displayHint(hintText);
              }
              break;

          case data::ItemType::Solve:
            setPreviousResult({data::Result::Status::Success, "Réponse révélée par l'item !"});
            setInterrogation(mQuizEngine->genererProchaineInterrogation());
            break;

          default:
            break;
        }
      } });

    // Quit button (dans la barre du haut)
    auto *stopButton = new QPushButton("Quitter", this);
    auto *btnDemarrer = new QPushButton("Démarrer le Quiz");
    connect(stopButton, &QPushButton::clicked, this, [this, btnDemarrer, stopButton]
            {
              if (stopButton->text() == "Quitter") {
              this->close(); // Ferme la fenêtre si aucune partie n'est en cours
            } else {
              // Sinon, termine la session
              mQuizEngine->endCurrentSession(data::GameStatus::Abandoned);
            } });

    connect(btnDemarrer, &QPushButton::clicked, this,
            [this, btnDemarrer, stopButton]
            {
              mQuizEngine->startNewGameSession();
              setInterrogation(mQuizEngine->genererProchaineInterrogation());
              btnDemarrer->hide(); // On cache le bouton une fois le quiz lancé
              mInventoryPanel->show();
              mDifficultyLabel->show();
              mLivesLabel->show();
              stopButton->setText("Terminer la partie");
            });
    topBar->addWidget(btnDemarrer, 0, Qt::AlignRight);
    topBar->addWidget(stopButton, 0, Qt::AlignRight);

    connect(this, &MainWindow::responseSelected, this,
            [this](data::Response response)
            {
              data::Result result = mQuizEngine->traiterReponse(response);
              // si l'utilisateur a perdu, on affiche le résultat final et on arrête là
              if (!mInventoryPanel->isVisible())
              {
                return; // On arrête l'exécution ici, pas de nouvelle question !
              }

              setPreviousResult(result);
              setInterrogation(mQuizEngine->genererProchaineInterrogation());
            });

    connect(mQuizEngine, &services::QuizEngine::sessionEnded, this,
            [this, btnDemarrer, stopButton](const data::GameSession & /*result*/) // <-- Ajout de stopButton ici
            {
              if (mInterrogation)
                mInterrogation->hide();
              if (mPreviousResult)
                mPreviousResult->hide();
              mInventoryPanel->hide();
              mDifficultyLabel->hide();
              mLivesLabel->hide();
              btnDemarrer->show();
              stopButton->setText("Quitter");
              mBossLabel->hide();
              mInventoryPanel->setEnabled(true);
            });

    mMainLayout->addLayout(topBar, 0, 0);

    mBossLabel = new QLabel("COMBAT DE BOSS (0/3)", this);
    mBossLabel->setAlignment(Qt::AlignCenter);
    mBossLabel->setStyleSheet("font-weight: bold; color: white; background-color: #c0392b; padding: 8px; font-size: 16px; border-radius: 5px;");
    mBossLabel->hide();
    mMainLayout->addWidget(mBossLabel, 1, 0);

    connect(mQuizEngine, &services::QuizEngine::bossStarted, this, [this]()
            {
              mBossLabel->setText("COMBAT DE BOSS (0/3)");
              mBossLabel->show();
              mInventoryPanel->setEnabled(false); // Désactive l'inventaire pendant le combat de boss
            });

    connect(mQuizEngine, &services::QuizEngine::bossProgressChanged, this, [this](int current, int target)
            { mBossLabel->setText(QString("COMBAT DE BOSS (%1/%2)").arg(current).arg(target)); });

    connect(mQuizEngine, &services::QuizEngine::bossEnded, this, [this](bool won)
            {
              mBossLabel->hide();
              mInventoryPanel->setEnabled(true); // Réactive l'inventaire après le boss
            });

    // This row is reserved for the previous result widget
    // The next one should be added at the mPreviousResultPosition.first + 1
    mPreviousResultPosition = {mMainLayout->rowCount(), 0};

    // This row is reserved for the interrogation widget
    // The next one should be added at the mInterrogationPosition.first + 1
    mInterrogationPosition = {mPreviousResultPosition.first + 1, 0};

    auto *centralWidget = new QWidget();
    centralWidget->setLayout(mMainLayout);
    setCentralWidget(centralWidget);

    mInventoryPanel->updateInventory(&mQuizEngine->getInventory());
  }

  void MainWindow::setInterrogation(const data::Interrogation &interrogation)
  {
    if (mInterrogation)
    {
      mMainLayout->removeWidget(mInterrogation);
      mInterrogation->deleteLater();
    }

    bool isBossFight = mQuizEngine->isBossActive();
    mInterrogation = new Interrogation(interrogation, mQuizEngine->getDerniereBonneReponse(), isBossFight);

    connect(mInterrogation, &Interrogation::responseSelected, this,
            &MainWindow::responseSelected);
    mMainLayout->addWidget(mInterrogation, mInterrogationPosition.first,
                           mInterrogationPosition.second);

    // Après avoir changé d'interrogation, on vérifie si le bonus 50/50 peut être utilisé
    if (mInventoryPanel)
    {
      mInventoryPanel->setItemUsable(data::ItemType::DeleteAnswer, mInterrogation->canHideWrongAnswer());
    }
  }

  void MainWindow::setPreviousResult(const data::Result &result)
  {
    if (mPreviousResult)
    {
      mMainLayout->removeWidget(mPreviousResult);
      mPreviousResult->deleteLater();
    }
    mPreviousResult = new PreviousResult(result);
    mMainLayout->addWidget(mPreviousResult, mPreviousResultPosition.first,
                           mPreviousResultPosition.second);
  }

  void ElCalculator::gui::MainWindow::closeEvent(QCloseEvent *event)
  {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString invFile = appDataPath + "/inventory.json";
    QString historyFile = appDataPath + "/history.json";

    if (mQuizEngine)
    {
      // Gestion d'une fermeture pendant une partie
      if (mQuizEngine->getLastSession() == std::nullopt)
      {
        mQuizEngine->endCurrentSession(data::GameStatus::Abandoned);
      }

      services::InventoryRepository::saveInventory(mQuizEngine->getInventory(), invFile);
      services::GameHistoryRepository::saveHistory(mQuizEngine->getHistory(), historyFile);
    }

    event->accept();
  }

} // namespace ElCalculator::gui
