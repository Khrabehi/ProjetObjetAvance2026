#pragma once

#include "data/Interrogation.hpp"
#include "data/Result.hpp"
#include "data/Inventory.hpp"
#include "data/Difficulty.hpp"
#include "data/GameSession.hpp"
#include "ILifePolicy.hpp"
#include <QObject>

#include <string>
#include <optional>
#include <memory>

namespace ElCalculator::services
{

    class QuizEngine : public QObject
    {
        Q_OBJECT
    public:
        // Constructeur explicit pour injecter la policy par défaut
        explicit QuizEngine(std::unique_ptr<ILifePolicy> lifePolicy = nullptr, QObject *parent = nullptr);
        // Gérer le cycle de vie d'une partie
        void startNewGameSession();
        void endCurrentSession(data::GameStatus status);
        // Génère une nouvelle interrogation à poser à l'utilisateur
        data::Interrogation genererProchaineInterrogation();

        // Evaluer la réponse de l'utilisateur et retourner le résultat de l'évaluation
        data::Result traiterReponse(const data::Response &reponse);

        bool useItem(data::ItemType type);

        const data::Inventory &getInventory() const;
        data::Inventory &getInventory();
        const data::Response &getDerniereBonneReponse() const;

        std::optional<data::GameSession> getLastSession() const { return mLastSession; }

        std::optional<data::GameSession> getBestSession() const;
        std::vector<data::GameSession> getTopScores(int n = 5) const;

        const std::vector<data::GameSession> &getHistory() const { return mHistory; }
        std::vector<data::GameSession> &getHistoryNonConst() { return mHistory; }
        void setHistory(const std::vector<data::GameSession> &history);
        bool isBossActive() const { return mBossActive; }

    signals:
        void inventoryUpdated(data::Inventory *inventory);      // Signal émis lorsque l'inventaire est mis à jour
        void difficultyChanged(data::Difficulty newDifficulty); // Signal émis lorsque la difficulté change
        void sessionEnded(data::GameSession finalResult);       // Signal émis à la fin d'une session de jeu
        void livesChanged(int remainingLives);
        void gameOver(data::GameSession finalResult);
        void bossStarted();
        void bossProgressChanged(int current, int target);
        void bossEnded(bool won);
        void gameStarted();
        void bossAboutToStart(int bossIndex);

    private:
        void lootItem(); // Fonction qui va attribuer aléatoirement un item
        void updateDifficulty();

        data::Response mDerniereBonneReponse;
        data::Inventory mInventory;
        int mStreak = 0; // Compteur de bonnes réponses à la suite
        data::Difficulty mCurrentDifficulty = data::Difficulty::Easy;
        std::optional<data::GameSession> mCurrentSession;
        std::optional<data::GameSession> mLastSession;
        std::vector<data::GameSession> mHistory;
        int mLives = 0;
        int mMaxLives = 5;
        std::unique_ptr<ILifePolicy> mLifePolicy;
        bool mSessionSaved = false;
        // Etat du boss
        bool mBossActive = false;
        int mBossCorrectStreak = 0;
        int mNextBossThreshold = 5; // Déclenchement toutes les 5s bonnes réponses
    };
} // namespace ElCalculator::services