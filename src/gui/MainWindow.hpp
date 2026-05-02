
#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QStandardPaths>

#include <gui/Interrogation.hpp>
#include <gui/PreviousResult.hpp>
#include "data/Difficulty.hpp"

#include <QGridLayout>
#include <QLabel>

#include <string>
#include <vector>

// Forward declaration pour éviter les dépendances circulaires
namespace ElCalculator::services
{
  class QuizEngine;
}

namespace ElCalculator::data
{
  class Interrogation;
} // namespace ElCalculator::data

namespace ElCalculator::gui
{

  class MainWindow : public QMainWindow
  {
    Q_OBJECT

  public:
    // Explicit est utilisé pour s'assurer que le constructeur ne peut pas être utilisé pour des conversions implicites.
    explicit MainWindow(services::QuizEngine *quizEngine);

    void setInterrogation(const data::Interrogation &interrogation);
    void setPreviousResult(const data::Result &result);

  Q_SIGNALS:
    void responseSelected(data::Response response);

  private:
    services::QuizEngine *mQuizEngine = nullptr;
    QGridLayout *mMainLayout = nullptr;
    QLabel *mDifficultyLabel = nullptr;
    std::pair<int, int> mInterrogationPosition;
    Interrogation *mInterrogation = nullptr;
    std::pair<int, int> mPreviousResultPosition;
    PreviousResult *mPreviousResult = nullptr;

  protected:
    void closeEvent(QCloseEvent *event) override;
  };

} // namespace ElCalculator::gui
