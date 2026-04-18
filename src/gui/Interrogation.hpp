
#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <data/Interrogation.hpp>

#include <string>
#include <vector>

namespace NomCool::gui
{

  class Interrogation : public QWidget
  {
    Q_OBJECT

  public:
    explicit Interrogation(const data::Interrogation &data, QWidget *parent = nullptr);
    void displayHint(const QString &texteIndice);

    void hideWrongAnswer();

  signals:
    void responseSelected(data::Response response);

  private:
    QLabel *mQuestionLabel = nullptr;
    QLabel *mHintLabel = nullptr;
    QVBoxLayout *mButtonsLayout = nullptr;
    std::vector<QPushButton *> mResponseButtons;
  };

} // namespace NomCool::gui
