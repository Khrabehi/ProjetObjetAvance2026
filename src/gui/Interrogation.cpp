#include "Interrogation.hpp"

#include <random>
#include <QDebug>

namespace ElCalculator::gui
{

  Interrogation::Interrogation(const data::Interrogation &data,
                               const data::Response &correctResponse,
                               QWidget *parent)
      : QWidget(parent), mCorrectResponse(QString::fromStdString(correctResponse))
  {
    auto *layout = new QVBoxLayout(this);

    // Question
    mQuestionLabel = new QLabel(QString::fromStdString(data.question()), this);
    mQuestionLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    layout->addWidget(mQuestionLabel);

    // Zone d'indice (cachée par défaut)
    mHintLabel = new QLabel(this);
    mHintLabel->setStyleSheet("color: #27ae60; font-style: italic;");
    mHintLabel->hide();
    layout->addWidget(mHintLabel);

    // Boutons de réponses
    for (const auto &[label, responseValue] : data.availableAnswers())
    {
      auto *btn = new QPushButton(QString::fromStdString(label), this);

      // On stocke la valeur de réponse dans une propriété Qt
      btn->setProperty("responseValue", QString::fromStdString(responseValue));

      connect(btn, &QPushButton::clicked, this, [this, btn]()
              { emit responseSelected(btn->property("responseValue").toString().toStdString()); });

      mResponseButtons.push_back(btn);
      layout->addWidget(btn);
    }
  }

  void Interrogation::displayHint(const QString &texteIndice)
  {
    if (mHintLabel)
    {
      mHintLabel->setText("Indice : " + texteIndice);
      mHintLabel->show();
    }
  }

  // Fonction qui cache une mauvaise réponse lorsque le bonus est utilisé
  bool Interrogation::hideWrongAnswer()
  {
    std::vector<QPushButton *> wrongButtons;
    for (auto *btn : mResponseButtons)
    {
      if (btn->isHidden())
      {
        continue;
      }
      const QString responseValue = btn->property("responseValue").toString();
      if (responseValue != mCorrectResponse)
      {
        wrongButtons.push_back(btn);
      }
    }

    if (wrongButtons.empty())
    {
      return false;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, wrongButtons.size() - 1);

    auto *chosen = wrongButtons[dis(gen)];
    chosen->hide();
    return true;
  }

  // Fonction qui vérifie si on peut utiliser le bonus 50/50 (s'il reste au moins une mauvaise réponse visible)
  bool Interrogation::canHideWrongAnswer() const
  {
    bool found = false;
    for (auto *btn : mResponseButtons)
    {
      if (btn->isHidden())
      {
        continue;
      }

      const QString responseValue = btn->property("responseValue").toString();
      if (responseValue != mCorrectResponse)
      {
        found = true;
        break;
      }
    }
    return found;
  }

} // namespace ElCalculator::gui