
#include "MainWindow.hpp"

#include <gui/Interrogation.hpp>

#include <data/Interrogation.hpp>

#include <QVBoxLayout>

// Debug
#include <QPushButton>
#include <random>
// End debug

namespace NomCool::gui {

// Debug
data::Interrogation createInterrogation() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  constexpr auto max = 10;
  std::uniform_int_distribution<> dis(1, max + 1);

  auto lhs = dis(gen);
  auto rhs = dis(gen);
  auto valuePlus1 = [](int value) {
    // For 10, we want 1, for other we want +1
    // max%max + 1 = 1 and for other x%max + 1 = x + 1
    return value % max + 1;
  };
  return {std::to_string(lhs) + " * " + std::to_string(rhs),
          {{std::to_string(lhs * rhs), "Correct"},
           {std::to_string(lhs * valuePlus1(rhs)), "Incorrect"},
           {std::to_string(valuePlus1(lhs) * rhs), "Incorrect"},
           {"I don't know", "I don't know"}}};
}
// End debug

MainWindow::MainWindow() {
  mMainLayout = new QGridLayout();

  // Debug
  auto *randomQuestion = new QPushButton("Random question");
  connect(randomQuestion, &QPushButton::clicked, this,
          [this] { setInterrogation(createInterrogation()); });
  mMainLayout->addWidget(randomQuestion, 0, 0);
  connect(this, &MainWindow::responseSelected, this,
          [this](data::Response response) {
            if (response == "I don't know") {
              setPreviousResult({data::Result::Status::Failure,
                                 "You will get it next time!"});
            } else if (response == "Correct") {
              setPreviousResult({data::Result::Status::Success, "Well done!"});
            } else {
              setPreviousResult(
                  {data::Result::Status::Failure, "No, it's not that one!"});
            }
            setInterrogation(createInterrogation());
          });
  // End debug

  // This row is reserved for the previous result widget
  // The next one should be added at the mPreviousResultPosition.first + 1
  mPreviousResultPosition = {mMainLayout->rowCount(), 0};

  // This row is reserved for the interrogation widget
  // The next one should be added at the mInterrogationPosition.first + 1
  mInterrogationPosition = {mPreviousResultPosition.first + 1, 0};

  // Debug
  // Quit button
  auto *quitButton = new QPushButton("Quit");
  connect(quitButton, &QPushButton::clicked, this, &MainWindow::close);
  mMainLayout->addWidget(quitButton, mInterrogationPosition.first + 1, 0);
  // End debug
  auto *centralWidget = new QWidget();
  centralWidget->setLayout(mMainLayout);
  setCentralWidget(centralWidget);
}

void MainWindow::setInterrogation(const data::Interrogation &interrogation) {
  if (mInterrogation) {
    mMainLayout->removeWidget(mInterrogation);
    delete mInterrogation;
  }
  mInterrogation = new Interrogation(interrogation);
  connect(mInterrogation, &Interrogation::responseSelected, this,
          &MainWindow::responseSelected);
  mMainLayout->addWidget(mInterrogation, mInterrogationPosition.first,
                         mInterrogationPosition.second);
}

void MainWindow::setPreviousResult(const data::Result &result) {
  if (mPreviousResult) {
    mMainLayout->removeWidget(mPreviousResult);
    delete mPreviousResult;
  }
  mPreviousResult = new PreviousResult(result);
  mMainLayout->addWidget(mPreviousResult, mPreviousResultPosition.first,
                         mPreviousResultPosition.second);
}

} // namespace NomCool::gui
