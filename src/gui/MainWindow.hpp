
#pragma once

#include <QMainWindow>

#include <gui/Interrogation.hpp>
#include <gui/PreviousResult.hpp>

#include <QGridLayout>
#include <QLabel>

#include <string>
#include <vector>

// Forward declaration pour éviter les dépendances circulaires
namespace NomCool::services {
class QuizEngine; 
}

namespace NomCool::data {
class Interrogation;
} // namespace NomCool::data

namespace NomCool::gui {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  // Explicit est utilisé pour s'assurer que le constructeur ne peut pas être utilisé pour des conversions implicites.
  explicit MainWindow(services::QuizEngine* quizEngine);

  void setInterrogation(const data::Interrogation &interrogation);
  void setPreviousResult(const data::Result &result);

Q_SIGNALS:
  void responseSelected(data::Response response);

private:
  services::QuizEngine* mQuizEngine = nullptr;
  QGridLayout *mMainLayout = nullptr;
  std::pair<int, int> mInterrogationPosition;
  Interrogation *mInterrogation = nullptr;
  std::pair<int, int> mPreviousResultPosition;
  PreviousResult *mPreviousResult = nullptr;
};

} // namespace NomCool::gui
