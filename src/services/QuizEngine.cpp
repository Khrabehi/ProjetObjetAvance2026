#include "QuizEngine.hpp"

#include <algorithm>
#include <random>
#include "ItemFactory.hpp"

namespace ElCalculator::services
{

    void QuizEngine::updateDifficulty()
    {
        data::Difficulty newDifficulty = data::Difficulty::Easy;
        if(mStreak >= 10) {
            newDifficulty = data::Difficulty::Expert;
        } else if(mStreak >= 6) {
            newDifficulty = data::Difficulty::Hard;
        } else if(mStreak >= 3) {
            newDifficulty = data::Difficulty::Medium;
        }

        if(newDifficulty != mCurrentDifficulty) {
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

        switch (mCurrentDifficulty) {
            case data::Difficulty::Easy:
                min = 1; max = 10; break;
            case data::Difficulty::Medium:
                min = 1; max = 20; break;
            case data::Difficulty::Hard:
                min = 5; max = 50; break;
            case data::Difficulty::Expert:
                min = 10; max = 99; break; // Multiplications à 2 chiffres
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
        if (reponse == mDerniereBonneReponse)
        {
            mStreak++;
            updateDifficulty();
            lootItem(); // Tente de looter un item après une bonne réponse
            return data::Result(data::Result::Status::Success, "Bonne réponse !");
        }
        else
        {
            mStreak = 0;
            updateDifficulty();
            return data::Result(data::Result::Status::Failure, "Mauvaise réponse. La bonne réponse était : " + mDerniereBonneReponse);
        }
    }

    void QuizEngine::lootItem()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);

        if (dis(gen) <= 30) // DEBUG 100%
        { // 30% de chance de looter un item
            auto item = ItemFactory::createRandomItem();
            mInventory.addItem(item->getId(), 1);
            emit inventoryUpdated(&mInventory); // Émettre le signal pour notifier les changements d'inventaire
        }
    }

    bool QuizEngine::useItem(data::ItemType type)
    {
        if (mInventory.getItemCount(type) > 0)
        {
            // On réduit le stock et on notifie.
            mInventory.removeItem(type, 1);
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
}