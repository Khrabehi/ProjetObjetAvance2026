
#pragma once

#include <QWidget>

#include <data/Result.hpp>

namespace ElCalculator::gui {

class PreviousResult : public QWidget {
  Q_OBJECT

public:
  PreviousResult(const data::Result &result);
};

} // namespace ElCalculator::gui
