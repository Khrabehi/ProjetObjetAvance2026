
#include "MainWindow.hpp"

#include "services/QuizEngine.hpp"

#include <gui/Interrogation.hpp>

#include <data/Interrogation.hpp>

#include <QVBoxLayout>

#include <stdexcept>

// Debug
#include <QPushButton>
#include <random>
// End debug

namespace NomCool::gui
{

  MainWindow::MainWindow(services::QuizEngine *quizEngine)
      : mQuizEngine(quizEngine)
  {

    if (!mQuizEngine)
    {
      throw std::invalid_argument("Erreur : MainWindow a reçu un QuizEngine nul.");
    }

    mMainLayout = new QGridLayout();

    auto *btnDemarrer = new QPushButton("Démarrer le Quiz");
    connect(btnDemarrer, &QPushButton::clicked, this,
            [this, btnDemarrer]
            {
              setInterrogation(mQuizEngine->genererProchaineInterrogation());
              btnDemarrer->hide(); // On cache le bouton une fois le quiz lancé
            });
    mMainLayout->addWidget(btnDemarrer, 0, 0);

    connect(this, &MainWindow::responseSelected, this,
            [this](data::Response response)
            {
              data::Result result = mQuizEngine->traiterReponse(response);
              setPreviousResult(result);

              setInterrogation(mQuizEngine->genererProchaineInterrogation());
            });

    // This row is reserved for the previous result widget
    // The next one should be added at the mPreviousResultPosition.first + 1
    mPreviousResultPosition = {mMainLayout->rowCount(), 0};

    // This row is reserved for the interrogation widget
    // The next one should be added at the mInterrogationPosition.first + 1
    mInterrogationPosition = {mPreviousResultPosition.first + 1, 0};

    // Quit button
    auto *quitButton = new QPushButton("Quit");
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::close);
    mMainLayout->addWidget(quitButton, mInterrogationPosition.first + 1, 0);

    auto *centralWidget = new QWidget();
    centralWidget->setLayout(mMainLayout);
    setCentralWidget(centralWidget);
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

} // namespace NomCool::gui
