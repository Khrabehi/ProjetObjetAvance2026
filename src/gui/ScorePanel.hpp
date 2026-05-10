#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "data/GameSession.hpp"

namespace ElCalculator::services
{
    class QuizEngine;
}

namespace ElCalculator::gui
{
    class ScorePanel : public QWidget
    {
        Q_OBJECT
    public:
        explicit ScorePanel(services::QuizEngine *engine, QWidget *parent = nullptr);
    public slots:
        // Signal appelé à la fin d'une partie ou au démarrage pour rafraîchir l'affichage
        void updateScores();

    private:
        services::QuizEngine *mEngine;

        QLabel *mLastRunLabel;
        QLabel *mBestScoreLabel;
        QLabel *mHistoryLabel;
    };
} // namespace ElCalculator::gui