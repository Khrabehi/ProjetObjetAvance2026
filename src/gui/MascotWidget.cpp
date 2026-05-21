#include "MascotWidget.hpp"
#include <QHBoxLayout>
#include <QSizePolicy>

namespace ElCalculator::gui
{
    MascotWidget::MascotWidget(QWidget *parent)
        : QWidget(parent)
    {
        auto *mainLayout = new QHBoxLayout(this);
        mainLayout->setContentsMargins(10, 10, 10, 10);
        mainLayout->setSpacing(12);

        mMascotAvatar = new QLabel(this);
        mMascotAvatar->setAlignment(Qt::AlignCenter);

        mMascotAvatar = new QLabel(this);
        mMascotAvatar->setAlignment(Qt::AlignCenter);

        QPixmap mascotImg(":/socrate.JPEG");

        if (!mascotImg.isNull())
        {
            mMascotAvatar->setPixmap(mascotImg.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        mMascotAvatar->setStyleSheet(
            "font-size: 32px; "
            "background-color: #e8f4f8; "
            "border-radius: 25px; "
            "padding: 5px; "
            "min-width: 50px; "
            "min-height: 50px; "
            "max-width: 50px; "
            "max-height: 50px;");

        auto *rightLayout = new QVBoxLayout();
        rightLayout->setSpacing(4);

        auto *topLayout = new QHBoxLayout();
        auto *titleLabel = new QLabel("<b>Socrate, le Chat</b>", this);
        titleLabel->setStyleSheet("color: #7f8c8d; font-size: 11px;");

        mHideButton = new QPushButton("×", this);
        mHideButton->setFixedSize(18, 18);
        mHideButton->setCursor(Qt::PointingHandCursor);
        mHideButton->setStyleSheet(
            "QPushButton { "
            "  border: none; "
            "  font-weight: bold; "
            "  color: #95a5a6; "
            "  font-size: 14px; "
            "} "
            "QPushButton:hover { "
            "  color: #c0392b; "
            "}");

        topLayout->addWidget(titleLabel);
        topLayout->addStretch();
        topLayout->addWidget(mHideButton);

        mDialogueBubble = new QLabel("...", this);
        mDialogueBubble->setWordWrap(true);
        mDialogueBubble->setMinimumWidth(240);
        mDialogueBubble->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        mDialogueBubble->setStyleSheet(
            "background-color: #ffffff; "
            "border: 1px solid #dcdde1; "
            "border-radius: 6px; "
            "padding: 8px; "
            "font-size: 12px; "
            "color: #2c3e50;");

        rightLayout->addLayout(topLayout);
        rightLayout->addWidget(mDialogueBubble);

        mainLayout->addWidget(mMascotAvatar, 0, Qt::AlignTop);
        mainLayout->addLayout(rightLayout, 1);

        setStyleSheet(
            "ElCalculator--gui--MascotWidget { "
            "  background-color: #f5f6fa; "
            "  border: 1px solid #dcdde1; "
            "  border-radius: 8px; "
            "}");

        connect(mHideButton, &QPushButton::clicked, this, &QWidget::hide);
    }

    void MascotWidget::setDialogue(const QString &text)
    {
        if (mDialogueBubble)
        {
            mDialogueBubble->setText(text);
            show();
        }
    }
} // namespace ElCalculator::gui