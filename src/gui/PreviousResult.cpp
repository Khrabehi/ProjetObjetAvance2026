
#include "PreviousResult.hpp"

#include <QHBoxLayout>
#include <QLabel>

namespace ElCalculator::gui {

PreviousResult::PreviousResult(const data::Result &result) {
  auto *layout = new QHBoxLayout();
  QIcon resultIcon(result.isSuccess() ? ":/icons/ok.png" : ":/icons/ko.png");
  QLabel *iconLabel = new QLabel();
  iconLabel->setPixmap(resultIcon.pixmap(32, 32));
  iconLabel->setAlignment(Qt::AlignCenter);
  layout->addWidget(iconLabel);
  auto *text = new QLabel();
  text->setText(QString::fromStdString(result.message()));
  layout->addWidget(text);
  setLayout(layout);
}

} // namespace ElCalculator::gui
