#pragma once 

#include "data/Item.hpp"
#include <iostream>

namespace NomCool::items
{
    class SolveItem : public data::Item {
    public:
        SolveItem() : data::Item(data::ItemType::Solve, "Résoudre", ":/sprites/solve.png") {}
        void use() override {
            std::cout << "Utilisation de Résoudre : affichage de la réponse correcte." << std::endl;
        }

        std::string describe() const override { return "Révèle la réponse."; }
        QString getIcon() const override { return mSprite; }
    };
}// namespace NomCool::items