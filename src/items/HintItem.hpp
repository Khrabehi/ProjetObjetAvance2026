#pragma once

#include "data/Item.hpp"
#include <iostream>

namespace NomCool::items
{
    class HintItem : public data::Item
    {
    public:
        HintItem() : data::Item(data::ItemType::Hint, "ndice", ":/sprites/hint.png") {}
        void use() override
        {
            std::cout << "Utilisation d'un Indice : affichage d'un calcul partiel." << std::endl;
        }
        std::string describe() const override { return "Affiche un calcul partiel pour vous aider."; }
        QString getIcon() const override { return mSprite; }
    };
} // namespace NomCool::items