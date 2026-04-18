#pragma once

#include "data/Interrogation.hpp"
#include "data/Result.hpp"
#include "data/Inventory.hpp"
#include <QObject>

#include <string>

namespace ElCalculator::services
{

    class QuizEngine : public QObject
    {
        Q_OBJECT
    public:
        // Génère une nouvelle interrogation à poser à l'utilisateur
        data::Interrogation genererProchaineInterrogation();

        // Evaluer la réponse de l'utilisateur et retourner le résultat de l'évaluation
        data::Result traiterReponse(const data::Response &reponse);

        bool useItem(data::ItemType type);

        const data::Inventory& getInventory() const;
        data::Inventory& getInventory();

    signals:
        void inventoryUpdated(data::Inventory *inventory); // Signal émis lorsque l'inventaire est mis à jour

    private:

        void lootItem(); // Fonction qui va attribuer aléatoirement un item
    
        data::Response mDerniereBonneReponse;
        data::Inventory mInventory;
    };
} // namespace ElCalculator::services