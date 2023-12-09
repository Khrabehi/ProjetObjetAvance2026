
#pragma once

#include <QWidget>

#include <data/Interrogation.hpp>

#include <string>
#include <vector>

namespace NomCool::gui {

class Interrogation : public QWidget {
  Q_OBJECT

public:
  Interrogation(const data::Interrogation &interrogation);

Q_SIGNALS:
  void responseSelected(data::Response response);
};

} // namespace NomCool::gui
