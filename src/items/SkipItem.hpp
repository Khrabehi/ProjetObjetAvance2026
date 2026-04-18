#pragma once

#include "data/Item.hpp"
#include <iostream>

namespace ElCalculator::items
{
    class SkipItem : public data::Item {
    public:
        SkipItem() : data::Item(data::ItemType::Skip, "Passer", ":/sprites/skip.png") {}
        void use() override {
            std::cout << "Utilisation de Passer : changement de question." << std::endl;
        }

        std::string describe() const override { return "Change la question en cours."; }
        QString getIcon() const override { return mSprite; }
    };
}// namespace ElCalculator::items