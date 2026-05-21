
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
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QSizePolicy>
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

    // ------------------BOSS LABEL------------------
    mBossLabel = new QLabel("COMBAT DE BOSS (0/3)", this);
    mBossLabel->setAlignment(Qt::AlignCenter);
    mBossLabel->setStyleSheet("font-weight: bold; color: white; background-color: #c0392b; padding: 8px; font-size: 16px; border-radius: 5px;");
    mBossLabel->hide();

    // ------------------CONTENU PRINCIPAL------------------
    auto *contentWidget = new QWidget(this);
    mGameContentLayout = new QVBoxLayout(contentWidget);
    mGameContentLayout->setSpacing(8);
    mGameContentLayout->addStretch();

    // ------------------PANEL PRINCIPAL------------------
    auto *centralWidget = new QWidget(this);
    auto *rootLayout = new QVBoxLayout(centralWidget);
    rootLayout->setContentsMargins(16, 16, 16, 16);
    rootLayout->setSpacing(12);

    //-------------------INVENTAIRE------------------
    mInventoryPanel = new InventoryWidget();
    mInventoryPanel->hide();
    mInventoryPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // ------------------SCORE PANEL------------------
    mScorePanel = new ScorePanel(mQuizEngine, this);
    mScorePanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mScorePanel->setMinimumWidth(340);
    mScorePanel->setMaximumWidth(420);
    mScorePanel->updateScores();

    // ------------------MASCOT------------------
    mMascotController = new services::MascotController();
    mMascotWidget = new MascotWidget(this);
    mMascotWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mMascotWidget->setMinimumWidth(340);
    mMascotWidget->setMaximumWidth(420);
    mMascotWidget->hide();

    //------------------TOP BAR------------------
    mToggleMascotBtn = new QPushButton("Masquer Mascotte", this);
    mToggleMascotBtn->setCheckable(true);
    connect(mToggleMascotBtn, &QPushButton::toggled, this, [this](bool checked)
            {
        mShowMascot = !checked; // Si le bouton est coché, on masque
        mToggleMascotBtn->setText(mShowMascot ? "Masquer Mascotte" : "Afficher Mascotte");
        
        if (!mShowMascot) {
            mMascotWidget->hide();
        } else if (mDifficultyLabel->isVisible()) { 
            mMascotWidget->show();
        } });

    connect(mQuizEngine, &services::QuizEngine::sessionEnded, mScorePanel, &ScorePanel::updateScores);

    connect(mQuizEngine, &services::QuizEngine::inventoryUpdated,
            mInventoryPanel, &InventoryWidget::updateInventory);

    mDifficultyLabel = new QLabel("Niveau : Facile", this);
    mDifficultyLabel->setAlignment(Qt::AlignCenter);
    mDifficultyLabel->setStyleSheet("font-weight: bold; color: #2980b9; font-size: 14px;");
    mDifficultyLabel->hide(); // On le cache tant que le quiz n'a pas démarré

    //-------------------LIVES LABEL------------------
    mLivesLabel = new QLabel("❤️ Vies : -", this);
    mLivesLabel->setAlignment(Qt::AlignCenter);
    mLivesLabel->setStyleSheet("font-weight: bold; color: #e74c3c; font-size: 14px;");
    mLivesLabel->hide(); // Caché avant le début du quiz

    // Top bar layout
    auto *topBarWidget = new QWidget(this);
    auto *topBar = new QHBoxLayout(topBarWidget);
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
    topBar->addWidget(mToggleMascotBtn, 0, Qt::AlignLeft);
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

    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(contentWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet("QScrollArea { border: none; }");

    auto *leftPanelWidget = new QWidget(this);
    auto *leftPanelLayout = new QVBoxLayout(leftPanelWidget);
    leftPanelLayout->setSpacing(12);
    leftPanelLayout->addWidget(topBarWidget);
    leftPanelLayout->addWidget(mBossLabel);
    leftPanelLayout->addWidget(scrollArea, 1);

    auto *rightPanelWidget = new QWidget(this);
    auto *rightPanelLayout = new QVBoxLayout(rightPanelWidget);
    rightPanelLayout->setSpacing(12);
    rightPanelLayout->addWidget(mScorePanel);
    rightPanelLayout->addWidget(mMascotWidget);
    rightPanelLayout->addStretch();

    auto *bodyWidget = new QWidget(this);
    auto *bodyLayout = new QHBoxLayout(bodyWidget);
    bodyLayout->setSpacing(12);
    bodyLayout->addWidget(leftPanelWidget, 5);
    bodyLayout->addWidget(rightPanelWidget, 2);

    rootLayout->addWidget(bodyWidget, 1);

    setMinimumSize(1300, 600);

    connect(mQuizEngine, &services::QuizEngine::bossStarted, this, [this, scrollArea]()
            {
              mBossLabel->setText("COMBAT DE BOSS (0/3)");
              mBossLabel->show();
              scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
              mInventoryPanel->setEnabled(false); // Désactive l'inventaire pendant le combat de boss
            });

    connect(mQuizEngine, &services::QuizEngine::bossProgressChanged, this, [this](int current, int target)
            { mBossLabel->setText(QString("COMBAT DE BOSS (%1/%2)").arg(current).arg(target)); });

    connect(mQuizEngine, &services::QuizEngine::bossEnded, this, [this, scrollArea](bool won)
            {
              mBossLabel->hide();
              scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
              mInventoryPanel->setEnabled(true); // Réactive l'inventaire après le boss
            });


    // Signal début de partie pour le premier dialogue de la mascotte
    connect(mQuizEngine, &services::QuizEngine::gameStarted, this, [this]()
            {
        if (mShowMascot) {
            mMascotWidget->setDialogue(mMascotController->getStartOfGameMessage());
            mMascotWidget->show();
        } });

    // Signal avant un combat de boss pour un dialogue d'introduction
    connect(mQuizEngine, &services::QuizEngine::bossAboutToStart, this, [this](int bossIndex)
            {
        if (mShowMascot) {
            mMascotWidget->setDialogue(mMascotController->getPreBossMessage(bossIndex));
            mMascotWidget->show();
        } });

    // Signal après un combat de boss pour un dialogue de conclusion
    connect(mQuizEngine, &services::QuizEngine::bossEnded, this, [this](bool won)
            {
        if (mShowMascot) {
            mMascotWidget->setDialogue(mMascotController->getPostBossMessage(won));
            mMascotWidget->show();
        } });

    setCentralWidget(centralWidget);

    mInventoryPanel->updateInventory(&mQuizEngine->getInventory());
  }

  void MainWindow::setInterrogation(const data::Interrogation &interrogation)
  {
    if (mInterrogation)
    {
      mGameContentLayout->removeWidget(mInterrogation);
      mInterrogation->deleteLater();
    }

    bool isBossFight = mQuizEngine->isBossActive();
    mInterrogation = new Interrogation(interrogation, mQuizEngine->getDerniereBonneReponse(), isBossFight);

    connect(mInterrogation, &Interrogation::responseSelected, this,
            &MainWindow::responseSelected);
    mInterrogation->setMinimumHeight(200);

    mGameContentLayout->insertWidget(0, mInterrogation);

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
      mGameContentLayout->removeWidget(mPreviousResult);
      mPreviousResult->deleteLater();
    }
    mPreviousResult = new PreviousResult(result);
    mGameContentLayout->insertWidget(1, mPreviousResult);
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
