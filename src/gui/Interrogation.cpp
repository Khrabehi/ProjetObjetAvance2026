#include "Interrogation.hpp"

namespace NomCool::gui
{

  Interrogation::Interrogation(const data::Interrogation &data, QWidget *parent)
      : QWidget(parent)
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

  void Interrogation::hideWrongAnswer()
  {
    for (auto *btn : mResponseButtons)
    {
      if (btn->isVisible())
      {
        btn->hide();
        break;
      }
    }
  }

} // namespace NomCool::gui