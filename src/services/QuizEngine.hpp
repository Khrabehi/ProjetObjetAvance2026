#pragma once

#include "data/Interrogation.hpp"
#include "data/Result.hpp"

#include <string>
#include <random>


namespace NomCool::services {

    class QuizEngine
    {
    public: 
        // Génère une nouvelle interrogation à poser à l'utilisateur
        data::Interrogation genererProchaineInterrogation();
        
        // Evaluer la réponse de l'utilisateur et retourner le résultat de l'évaluation
        data::Result traiterReponse(const data::Response& reponse);

    private: 
        data::Response mDerniereBonneReponse; 
    };
} // namespace NomCool::services