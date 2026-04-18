#pragma once

#include "Item.hpp"
#include <map>
#include <vector>
#include <memory>

namespace ElCalculator::data
{
    class Inventory
    {
    public:
        Inventory();

        void addItem(ItemType type, int count);
        bool removeItem(ItemType type, int count);
        int getItemCount(ItemType type) const;

        std::vector<Item *> getItemsList() const;

    private:
        std::map<ItemType, std::unique_ptr<Item>> mItems;
    };
} // namespace ElCalculator::data