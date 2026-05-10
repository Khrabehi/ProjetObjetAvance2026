#include "GameHistoryRepository.hpp"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDateTime>

namespace ElCalculator::services
{
    bool GameHistoryRepository::saveHistory(const std::vector<data::GameSession> &history, const QString &filePath)
    {
        QJsonArray rootArray;

        // On parcourt l'historique
        int count = 0;
        for (auto it = history.rbegin(); it != history.rend() && count < MAX_HISTORY_SIZE; ++it, ++count)
        {
            const auto &session = *it;
            QJsonObject obj;

            obj["startedAt"] = QString::fromStdString(std::to_string(std::chrono::system_clock::to_time_t(session.startedAt))); // Converstion du time en timestamp
            obj["score"] = session.score;
            obj["correctAnswers"] = session.correctAnswers;
            obj["wrongAnswers"] = session.wrongAnswers;
            obj["itemsUsed"] = session.itemsUsed;
            obj["durationSeconds"] = session.durationSeconds;
            obj["status"] = static_cast<int>(session.finalStatus);

            rootArray.append(obj);
        }

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly))
            return false;

        file.write(QJsonDocument(rootArray).toJson());
        return true;
    }

    std::vector<data::GameSession> GameHistoryRepository::loadHistory(const QString &filePath)
    {
        std::vector<data::GameSession> history;
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
            return history;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isArray())
            return history;

        QJsonArray rootArray = doc.array();
        for (const auto &val : rootArray)
        {
            QJsonObject obj = val.toObject();
            data::GameSession session;

            // Reconstitution de l'objet data::GameSession
            session.startedAt = std::chrono::system_clock::from_time_t(obj["startedAt"].toString().toLongLong());
            session.score = obj["score"].toInt();
            session.correctAnswers = obj["correctAnswers"].toInt();
            session.wrongAnswers = obj["wrongAnswers"].toInt();
            session.itemsUsed = obj["itemsUsed"].toInt();
            session.durationSeconds = obj["durationSeconds"].toInt();
            session.finalStatus = static_cast<data::GameStatus>(obj["status"].toInt());

            history.push_back(session);
        }

        return history;
    }

} // namespace ElCalculator::services