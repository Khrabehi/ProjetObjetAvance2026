#pragma once

#include <vector>
#include <QString>
#include "data/GameSession.hpp"

namespace ElCalculator::services
{

    class GameHistoryRepository
    {
    public:
        static bool saveHistory(const std::vector<data::GameSession> &history, const QString &filePath);
        static std::vector<data::GameSession> loadHistory(const QString &filePath);
    private:
     static constexpr int MAX_HISTORY_SIZE = 50; // Limite de stockage des parties
    };
} // namespace ElCalculator::services