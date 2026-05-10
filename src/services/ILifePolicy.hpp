#pragma once

namespace ElCalculator::services {

struct LifeUpdateResult {
    int newLives = 0;
    int livesGained = 0;
    int livesLost = 0;
    bool isGameOver = false;
};

class ILifePolicy {
public:
    virtual ~ILifePolicy() = default;

    virtual int initialize() = 0;

    virtual LifeUpdateResult evaluateAnswer(bool isCorrect, int currentLives, int maxLives) = 0;
};

} // namespace ElCalculator::services