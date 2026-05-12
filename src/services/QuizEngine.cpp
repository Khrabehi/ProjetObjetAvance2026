#include "QuizEngine.hpp"

#include <algorithm>
#include <random>
#include "ItemFactory.hpp"
#include "StreakLifePolicy.hpp"

namespace ElCalculator::services
{

    QuizEngine::QuizEngine(std::unique_ptr<ILifePolicy> lifePolicy, QObject *parent)
        : QObject(parent), mLifePolicy(std::move(lifePolicy))
    {

        // Si aucune policy n'est injectée, on utilise la règle par défaut
        if (!mLifePolicy)
        {
            mLifePolicy = std::make_unique<StreakLifePolicy>(3); // 3 vies par défaut
        }
    }

    std::optional<data::GameSession> QuizEngine::getBestSession() const
    {
        if (mHistory.empty())
        {
            return std::nullopt;
        }

        // Trouve la session avec le meilleur score, en cas d'égalité, celle avec la durée la plus courte
        auto it = std::max_element(mHistory.begin(), mHistory.end(),
                                   [](const data::GameSession &a, const data::GameSession &b)
                                   {
                                       if (a.score != b.score)
                                           return a.score < b.score;
                                       return a.durationSeconds > b.durationSeconds; // En cas d'égalité, le plus rapide gagne
                                   });

        return *it;
    }

    std::vector<data::GameSession> QuizEngine::getTopScores(int n) const
    {
        if (mHistory.empty())
            return {};

        // Copie de l'historique pour ne pas modifier l'ordre chronologique de mHistory
        std::vector<data::GameSession> sortedHistory = mHistory;

        // Tri décroissant
        std::sort(sortedHistory.begin(), sortedHistory.end(),
                  [](const data::GameSession &a, const data::GameSession &b)
                  {
                      if (a.score != b.score)
                          return a.score > b.score;
                      return a.durationSeconds < b.durationSeconds;
                  });

        // On ne garde que les N premiers
        if (static_cast<int>(sortedHistory.size()) > n)
        {
            sortedHistory.resize(n);
        }

        return sortedHistory;
    }

    void QuizEngine::startNewGameSession()
    {
        mLives = mLifePolicy->initialize();
        mStreak = 0;
        mSessionSaved = false; // Reset du flag de sauvegarde

        // Reset de l'état Boss pour la nouvelle partie
        mBossActive = false;
        mBossCorrectStreak = 0;
        mNextBossThreshold = 5;

        data::GameSession session;
        session.startedAt = std::chrono::system_clock::now();
        session.initialLives = mLives;
        session.remainingLives = mLives;

        mCurrentSession = session;

        emit livesChanged(mLives);
    }

    void QuizEngine::endCurrentSession(data::GameStatus status)
    {
        if (!mCurrentSession)
            return;

        mCurrentSession->endedAt = std::chrono::system_clock::now();
        mCurrentSession->computeDuration();
        mCurrentSession->computeFinalScore();

        // Si on termine sans sauvegarde explicite, on force le statut Abandoned
        if (!mSessionSaved && status != data::GameStatus::Failure)
        {
            mCurrentSession->finalStatus = data::GameStatus::Abandoned;
            mCurrentSession->endedWithoutSave = true;
        }
        else
        {
            mCurrentSession->finalStatus = status;
            mCurrentSession->endedWithoutSave = false;
        }

        // Ajout à l'historique
        mHistory.push_back(*mCurrentSession);
        mLastSession = mCurrentSession;

        emit sessionEnded(*mCurrentSession);
        mCurrentSession.reset();
    }

    void QuizEngine::updateDifficulty()
    {
        data::Difficulty newDifficulty = data::Difficulty::Easy;
        if (mStreak >= 10)
        {
            newDifficulty = data::Difficulty::Expert;
        }
        else if (mStreak >= 6)
        {
            newDifficulty = data::Difficulty::Hard;
        }
        else if (mStreak >= 3)
        {
            newDifficulty = data::Difficulty::Medium;
        }

        if (newDifficulty != mCurrentDifficulty)
        {
            mCurrentDifficulty = newDifficulty;
            emit difficultyChanged(mCurrentDifficulty);
        }
    }

    /// Génère la prochaine interrogation du quiz de manière aléatoire.
    data::Interrogation QuizEngine::genererProchaineInterrogation()
    {

        static std::random_device randomDevice;              // Générateur de nombres aléatoires pour sélectionner une question
        static std::mt19937 randomGenerator(randomDevice()); // Moteur de génération de nombres aléatoires

        int min = 1;
        int max = 10;

        switch (mCurrentDifficulty)
        {
        case data::Difficulty::Easy:
            min = 1;
            max = 10;
            break;
        case data::Difficulty::Medium:
            min = 1;
            max = 20;
            break;
        case data::Difficulty::Hard:
            min = 5;
            max = 50;
            break;
        case data::Difficulty::Expert:
            min = 10;
            max = 99;
            break; // Multiplications à 2 chiffres
        }

        std::uniform_int_distribution<> dis(min, max); // Distribution de min à max inclus

        int premierFacteur = dis(randomGenerator);
        int deuxiemeFacteur = dis(randomGenerator);
        auto valuePlus1 = [max, min](int valeur)
        {
            return (valeur < max) ? valeur + 1 : min;
        };

        mDerniereBonneReponse = std::to_string(premierFacteur * deuxiemeFacteur); // Stocke la bonne réponse pour l'évaluation future

        // Construire les 4 options (valeur numérique uniquement) avant de les mélanger
        std::vector<std::string> valeurs = {
            std::to_string(premierFacteur * valuePlus1(deuxiemeFacteur)),
            std::to_string(valuePlus1(premierFacteur) * deuxiemeFacteur),
            mDerniereBonneReponse,
            std::to_string(valuePlus1(premierFacteur) * valuePlus1(deuxiemeFacteur))};

        // Mélanger l'ordre des options aléatoirement
        std::shuffle(valeurs.begin(), valeurs.end(), randomGenerator);

        // Construire les paires (libellé, réponse) en renumérotant après le mélange
        std::vector<std::pair<std::string, data::Response>> propositions;
        propositions.reserve(valeurs.size());
        for (std::size_t i = 0; i < valeurs.size(); ++i)
        {
            propositions.emplace_back("Option " + std::to_string(i + 1) + ": " + valeurs[i], valeurs[i]);
        }

        return data::Interrogation(
            "Combien font " + std::to_string(premierFacteur) + " x " + std::to_string(deuxiemeFacteur) + " ?",
            propositions);
    }

    // Traite la réponse de l'utilisateur en la comparant à la dernière bonne réponse stockée et retourne un résultat d'évaluation
    data::Result QuizEngine::traiterReponse(const data::Response &reponse)
    {
        bool isCorrect = (reponse == mDerniereBonneReponse);

        LifeUpdateResult lifeResult = mLifePolicy->evaluateAnswer(isCorrect, mLives, mMaxLives);

        if (mCurrentSession)
        {
            if (isCorrect)
                mCurrentSession->correctAnswers++;
            else
                mCurrentSession->wrongAnswers++;

            mCurrentSession->livesGained += lifeResult.livesGained;
            mCurrentSession->livesLost += lifeResult.livesLost;
            mCurrentSession->remainingLives = lifeResult.newLives;
        }
        mLives = lifeResult.newLives;
        emit livesChanged(mLives);

        if (lifeResult.isGameOver)
        {
            endCurrentSession(data::GameStatus::Failure); // Fin immédiate
            return data::Result(data::Result::Status::Failure, "GAME OVER : Vous n'avez plus de vies !");
        }

        // Logique boss fight
        if (mBossActive)
        {
            if (isCorrect)
            {
                mBossCorrectStreak++;
                emit bossProgressChanged(mBossCorrectStreak, 3);

                if (mBossCorrectStreak == 3)
                {
                    // Victoire du Boss
                    mBossActive = false;
                    mNextBossThreshold += 5;
                    if (mCurrentSession)
                        mCurrentSession->bossFightsWon++;

                    // Gain garanti d'un Skip
                    mInventory.addItem(data::ItemType::Skip, 1);
                    emit inventoryUpdated(&mInventory);

                    emit bossEnded(true);
                    return data::Result(data::Result::Status::Success, "BOSS VAINCU ! Vous gagnez un Skip.");
                }
                return data::Result(data::Result::Status::Success, "Correct ! Encore " + std::to_string(3 - mBossCorrectStreak));
            }
            else
            {
                // Erreur pendant le boss : reset de la série boss
                mBossCorrectStreak = 0;
                emit bossProgressChanged(0, 3);
                return data::Result(data::Result::Status::Failure, "Raté ! Le boss résiste, la série repart à zéro.");
            }
        }

        // Logique mode classique question + réponse
        if (isCorrect)
        {
            mStreak++;
            updateDifficulty();
            lootItem();

            // Vérifie si on atteint le seuil pour déclencher un boss fight
            if (mCurrentSession && mCurrentSession->correctAnswers >= mNextBossThreshold)
            {
                mBossActive = true;
                mBossCorrectStreak = 0;
                if (mCurrentSession) mCurrentSession->bossFightsStarted++;
                
                emit bossStarted();
                
                return data::Result(data::Result::Status::Success, "Bonne réponse... MAIS UN BOSS APPARAÎT !");
            }

            return data::Result(data::Result::Status::Success, "Bonne réponse !");
        }
        else
        {
            mStreak = 0;
            updateDifficulty();
            return data::Result(data::Result::Status::Failure, "Mauvaise réponse.");
        }
    }

    void QuizEngine::lootItem()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);

        if (dis(gen) <= 30) // DEBUG 100%
        {                   // 30% de chance de looter un item
            auto item = ItemFactory::createRandomItem();
            mInventory.addItem(item->getId(), 1);
            emit inventoryUpdated(&mInventory); // Émettre le signal pour notifier les changements d'inventaire
        }
    }

    bool QuizEngine::useItem(data::ItemType type)
    {

        if (mBossActive)
        {
            return false;
        }

        if (mInventory.getItemCount(type) > 0)
        {
            // On réduit le stock et on notifie.
            mInventory.removeItem(type, 1);
            if (mCurrentSession)
            {
                mCurrentSession->itemsUsed++;
            }
            emit inventoryUpdated(&mInventory);
            return true;
        }
        return false;
    }

    const data::Inventory &QuizEngine::getInventory() const
    {
        return mInventory;
    }

    data::Inventory &QuizEngine::getInventory()
    {
        return mInventory;
    }

    const data::Response &QuizEngine::getDerniereBonneReponse() const
    {
        return mDerniereBonneReponse;
    }

    void QuizEngine::setHistory(const std::vector<data::GameSession> &history)
    {
        mHistory = history;
        mLastSession = mHistory.empty() ? std::nullopt : std::optional<data::GameSession>(mHistory.back());
    }
}