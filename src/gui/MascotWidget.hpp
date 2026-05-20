#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace ElCalculator::gui
{
    class MascotWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit MascotWidget(QWidget *parent = nullptr);

        void setDialogue(const QString &text);

    private:
        QLabel *mMascotAvatar = nullptr;
        QLabel *mDialogueBubble = nullptr;
        QPushButton *mHideButton = nullptr;
    };
} // namespace ElCalculator::gui