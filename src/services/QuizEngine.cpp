#include "QuizEngine.hpp"

namespace NomCool::services {

    /// Génère la prochaine interrogation du quiz de manière aléatoire.
    data::Interrogation QuizEngine::genererProchaineInterrogation() {
        
        static std::random_device randomDevice; // Générateur de nombres aléatoires pour sélectionner une question
        static std::mt19937 randomGenerator(randomDevice()); // Moteur de génération de nombres aléatoires
        constexpr int max = 10;
        std::uniform_int_distribution<> dis(1, max + 1); // Distribution uniforme pour sélectionner une question entre 1 et max

        int premierFacteur = dis(randomGenerator);
        int deuxiemeFacteur = dis(randomGenerator);
        auto valuePlus1 = [](int valeur) {
            return valeur % max + 1;
        };

        mDerniereBonneReponse = std::to_string(premierFacteur * deuxiemeFacteur); // Stocke la bonne réponse pour l'évaluation future
        return data::Interrogation(
            "Combien font " + std::to_string(premierFacteur) + " x " + std::to_string(deuxiemeFacteur) + " ?",
            {
                {"Option 1: " + std::to_string(premierFacteur * valuePlus1(deuxiemeFacteur)), std::to_string(premierFacteur * valuePlus1(deuxiemeFacteur))},
                {"Option 2: " + std::to_string(valuePlus1(premierFacteur) * deuxiemeFacteur), std::to_string(valuePlus1(premierFacteur) * deuxiemeFacteur)},
                {"Option 3: " + std::to_string(premierFacteur * deuxiemeFacteur), mDerniereBonneReponse},
                {"Option 4: " + std::to_string(valuePlus1(premierFacteur) * valuePlus1(deuxiemeFacteur)), std::to_string(valuePlus1(premierFacteur) * valuePlus1(deuxiemeFacteur))}
            }
        );
    }

    // Traite la réponse de l'utilisateur en la comparant à la dernière bonne réponse stockée et retourne un résultat d'évaluation
    data::Result QuizEngine::traiterReponse(const data::Response& reponse) {
        if (reponse == mDerniereBonneReponse) {
            return data::Result(data::Result::Status::Success, "Bonne réponse !");
        } else {
            return data::Result(data::Result::Status::Failure, "Mauvaise réponse. La bonne réponse était : " + mDerniereBonneReponse);
        }
    }
}