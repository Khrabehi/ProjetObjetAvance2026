
#pragma once

#include <QMainWindow>
#include <QCloseEvent>
#include <QStandardPaths>

#include <gui/Interrogation.hpp>
#include <gui/PreviousResult.hpp>
#include "InventoryWidget.hpp"
#include "data/Difficulty.hpp"
#include "ScorePanel.hpp"

#include <gui/MascotWidget.hpp>
#include "services/MascotController.hpp"

#include <QLabel>
#include <QVBoxLayout>

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
    QGridLayout *mMainLayout = nullptr; // Legacy, no longer used
    QVBoxLayout *mGameContentLayout = nullptr; // For dynamic question/result widgets
    InventoryWidget *mInventoryPanel = nullptr;
    QLabel *mDifficultyLabel = nullptr;
    Interrogation *mInterrogation = nullptr;
    PreviousResult *mPreviousResult = nullptr;
    ScorePanel *mScorePanel = nullptr;
    QLabel *mLivesLabel = nullptr;
    QLabel *mBossLabel = nullptr;
    MascotWidget *mMascotWidget = nullptr;
    services::MascotController *mMascotController = nullptr;
    QPushButton *mToggleMascotBtn = nullptr;
    bool mShowMascot = true; 

  protected:
    void closeEvent(QCloseEvent *event) override;
  };

} // namespace ElCalculator::gui
