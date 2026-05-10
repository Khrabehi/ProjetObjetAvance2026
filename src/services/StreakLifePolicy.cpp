#include "StreakLifePolicy.hpp"
#include <algorithm>

namespace ElCalculator::services {

StreakLifePolicy::StreakLifePolicy(int initialLives)
    : mInitialLives(initialLives) {}

int StreakLifePolicy::initialize() {
    mCurrentStreak = 0; // On reset la série de bonnes réponses au début
    return mInitialLives;
}

LifeUpdateResult StreakLifePolicy::evaluateAnswer(bool isCorrect, int currentLives, int maxLives) {
    LifeUpdateResult result;
    result.newLives = currentLives;

    if (isCorrect) {
        mCurrentStreak++;
        // +1 vie si streak % 5 == 0
        if (mCurrentStreak > 0 && mCurrentStreak % 5 == 0) {
            result.livesGained = 1;
            // Règle: clamp des vies entre 0 et max
            result.newLives = std::min(currentLives + 1, maxLives);
        }
    } else {
        mCurrentStreak = 0;
        result.livesLost = 1;
        result.newLives = std::max(currentLives - 1, 0);
    }

    result.isGameOver = (result.newLives <= 0); // Trigger du Game Over
    return result;
}

} // namespace ElCalculator::services