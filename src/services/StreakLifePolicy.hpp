#pragma once

#include "ILifePolicy.hpp"

namespace ElCalculator::services {

class StreakLifePolicy : public ILifePolicy {
public:
    StreakLifePolicy(int initialLives);

    int initialize() override;
    LifeUpdateResult evaluateAnswer(bool isCorrect, int currentLives, int maxLives) override;

private:
    int mInitialLives;
    int mCurrentStreak = 0;
};

} // namespace ElCalculator::services