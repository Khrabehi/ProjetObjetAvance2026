
#pragma once

#include <QWidget>

#include <data/Result.hpp>

namespace NomCool::gui {

class PreviousResult : public QWidget {
  Q_OBJECT

public:
  PreviousResult(const data::Result &result);
};

} // namespace NomCool::gui
