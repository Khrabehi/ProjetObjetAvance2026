#include "QuizEngine.hpp"

#include <algorithm>
#include <random>

namespace NomCool::services {

    /// Génère la prochaine interrogation du quiz de manière aléatoire.
    data::Interrogation QuizEngine::genererProchaineInterrogation() {
        
        static std::random_device randomDevice; // Générateur de nombres aléatoires pour sélectionner une question
        static std::mt19937 randomGenerator(randomDevice()); // Moteur de génération de nombres aléatoires
        constexpr int max = 10;
        std::uniform_int_distribution<> dis(1, max); // Distribution de 1 à 10 inclus
        
        int premierFacteur = dis(randomGenerator);
        int deuxiemeFacteur = dis(randomGenerator);
        auto valuePlus1 = [](int valeur) {
            return valeur % max + 1;
        };

        mDerniereBonneReponse = std::to_string(premierFacteur * deuxiemeFacteur); // Stocke la bonne réponse pour l'évaluation future

        // Construire les 4 options (valeur numérique uniquement) avant de les mélanger
        std::vector<std::string> valeurs = {
            std::to_string(premierFacteur * valuePlus1(deuxiemeFacteur)),
            std::to_string(valuePlus1(premierFacteur) * deuxiemeFacteur),
            mDerniereBonneReponse,
            std::to_string(valuePlus1(premierFacteur) * valuePlus1(deuxiemeFacteur))
        };

        // Mélanger l'ordre des options aléatoirement
        std::shuffle(valeurs.begin(), valeurs.end(), randomGenerator);

        // Construire les paires (libellé, réponse) en renumérotant après le mélange
        std::vector<std::pair<std::string, data::Response>> propositions;
        propositions.reserve(valeurs.size());
        for (std::size_t i = 0; i < valeurs.size(); ++i) {
            propositions.emplace_back("Option " + std::to_string(i + 1) + ": " + valeurs[i], valeurs[i]);
        }

        return data::Interrogation(
            "Combien font " + std::to_string(premierFacteur) + " x " + std::to_string(deuxiemeFacteur) + " ?",
            propositions
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