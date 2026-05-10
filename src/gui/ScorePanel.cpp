#include "ScorePanel.hpp"
#include "services/QuizEngine.hpp"
#include <QString>

namespace ElCalculator::gui
{
    ScorePanel::ScorePanel(services::QuizEngine *engine, QWidget *parent)
        : QWidget(parent), mEngine(engine)
    {

        auto *layout = new QVBoxLayout(this);

        // Initialisation des labels
        QLabel *title = new QLabel("<b>Tableau des Scores</b>", this);
        title->setAlignment(Qt::AlignCenter);

        mLastRunLabel = new QLabel("Dernière partie : Aucune", this);
        mBestScoreLabel = new QLabel("Meilleur score : Aucun", this);
        mBestScoreLabel->setStyleSheet("color: #d35400; font-weight: bold;");

        mHistoryLabel = new QLabel("Historique récent :\n-", this);

        layout->addWidget(title);
        layout->addWidget(mLastRunLabel);
        layout->addWidget(mBestScoreLabel);
        layout->addWidget(mHistoryLabel);
    }

    void ScorePanel::updateScores()
    {
        if (!mEngine)
            return;

        // Dernière partie
        auto lastSession = mEngine->getLastSession();
        if (lastSession)
        {
            QString statusText = (lastSession->finalStatus == data::GameStatus::Success) ? "Terminée" : "Abandonnée";
            mLastRunLabel->setText(QString("Dernière partie : %1 pts (%2 bonnes réponses, %3s) - %4")
                                       .arg(lastSession->score)
                                       .arg(lastSession->correctAnswers)
                                       .arg(lastSession->durationSeconds)
                                       .arg(statusText));
        }

        // Meilleur score
        auto bestSession = mEngine->getBestSession();
        if (bestSession)
        {
            mBestScoreLabel->setText(QString("Meilleur score : %1 pts (%2s)")
                                         .arg(bestSession->score)
                                         .arg(bestSession->durationSeconds));
        }

        // Historique récents
        auto history = mEngine->getHistory();
        QString historyText = "Historique récent :\n";
        int count = 0;
        for (auto it = history.rbegin(); it != history.rend() && count < 5; ++it, ++count)
        {
            historyText += QString("- %1 pts (%2 items utilisés)\n")
                               .arg(it->score)
                               .arg(it->itemsUsed);
        }
        mHistoryLabel->setText(historyText);
    }
}