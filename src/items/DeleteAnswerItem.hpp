#pragma once 

#include "data/Item.hpp"
#include <iostream>

namespace NomCool::items
{
    class DeleteAnswerItem : public data::Item {
    public:
        DeleteAnswerItem() : data::Item(data::ItemType::DeleteAnswer, "50/50", ":/sprites/delete_answer.png") {}
        void use() override {
            std::cout << "Utilisation de 50/50 : supprime une mauvaise réponse." << std::endl;
        }

        std::string describe() const override { return "Supprime une mauvaise réponse."; }
        QString getIcon() const override { return mSprite; }
    };
}// namespace NomCool::items