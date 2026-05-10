
#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <data/Interrogation.hpp>

#include <string>
#include <vector>

namespace ElCalculator::gui
{

  class Interrogation : public QWidget
  {
    Q_OBJECT

  public:
    explicit Interrogation(const data::Interrogation &data,
                 const data::Response &correctResponse,
                 QWidget *parent = nullptr);
    void displayHint(const QString &texteIndice);
    bool hideWrongAnswer();
    bool canHideWrongAnswer() const;

  signals:
    void responseSelected(data::Response response);

  private:
    QLabel *mQuestionLabel = nullptr;
    QLabel *mHintLabel = nullptr;
    QVBoxLayout *mButtonsLayout = nullptr;
    std::vector<QPushButton *> mResponseButtons;
    QString mCorrectResponse;
  };

} // namespace ElCalculator::gui
