
#include "Interrogation.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

namespace NomCool::gui {

Interrogation::Interrogation(const data::Interrogation &interrogation) {
  auto *mainLayout = new QVBoxLayout();
  auto *questionLabel = new QLabel();
  questionLabel->setText(QString::fromStdString(interrogation.question()));
  mainLayout->addWidget(questionLabel);
  auto *solutionLayout = new QGridLayout();
  for (const auto &proposedAnswer : interrogation.availableAnswers()) {
    auto *button =
        new QPushButton(QString::fromStdString(proposedAnswer.first));
    connect(button, &QPushButton::clicked, this, [this, proposedAnswer] {
      emit responseSelected(proposedAnswer.second);
    });
    solutionLayout->addWidget(button);
  }
  mainLayout->addLayout(solutionLayout);
  setLayout(mainLayout);
}

} // namespace NomCool::gui
