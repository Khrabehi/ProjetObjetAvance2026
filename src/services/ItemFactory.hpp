#pragma once 
#include <memory>
#include "data/Item.hpp"

namespace ElCalculator::services {

    class ItemFactory{
        public:
            static std::unique_ptr<data::Item> createItem(data::ItemType type);
            static std::unique_ptr<data::Item> createRandomItem();
    };
}