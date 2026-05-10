#pragma once

#include <chrono>
#include <string>

namespace ElCalculator::data
{
    enum class GameStatus
    {
        Success,
        Failure,
        Abandoned
    };

    struct GameSession
    {
        // Horodatage de départ et de fin
        std::chrono::system_clock::time_point startedAt;
        std::chrono::system_clock::time_point endedAt;

        // Statistiques de la partie
        int durationSeconds = 0;
        int correctAnswers = 0;
        int wrongAnswers = 0;
        int itemsUsed = 0;

        // Suivi des vies
        int initialLives = 0;
        int remainingLives = 0;
        int livesLost = 0;
        int livesGained = 0;

        bool endedWithoutSave = true;

        // Résultat
        int score = 0;
        GameStatus finalStatus = GameStatus::Abandoned; // Abandonné par défaut si la partie crash

        void computeFinalScore()
        {
            // Règle : Score = bonnes réponses × 10 - mauvaises réponses × 2
            score = (correctAnswers * 10) - (wrongAnswers * 2);

            if (score < 0)
            {
                score = 0;
            }
        }

        void computeDuration()
        {
            durationSeconds = std::chrono::duration_cast<std::chrono::seconds>(endedAt - startedAt).count();
        }
    };
} // namespace ElCalculator::data