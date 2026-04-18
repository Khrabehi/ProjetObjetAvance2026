#include "Inventory.hpp"

#include "items/HintItem.hpp"
#include "items/SkipItem.hpp"
#include "items/SolveItem.hpp"
#include "items/DeleteAnswerItem.hpp"

namespace ElCalculator::data
{
    Inventory::Inventory()
    {
        mItems.emplace(ItemType::Hint, std::make_unique<items::HintItem>());
        mItems.emplace(ItemType::Skip, std::make_unique<items::SkipItem>());
        mItems.emplace(ItemType::Solve, std::make_unique<items::SolveItem>());
        mItems.emplace(ItemType::DeleteAnswer, std::make_unique<items::DeleteAnswerItem>());
    }

    void Inventory::addItem(ItemType type, int count)
    {
        if (mItems.find(type) != mItems.end())
        {
            mItems[type]->addAmount(count);
        }
    }

    bool Inventory::removeItem(ItemType type, int count)
    {
        if (mItems.find(type) != mItems.end())
        {
            return mItems[type]->removeAmount(count);
        }
        return false;
    }

    int Inventory::getItemCount(ItemType type) const
    {
        if (mItems.find(type) != mItems.end())
        {
            return mItems.at(type)->getCount();
        }
        return 0;
    }

    std::vector<Item *> Inventory::getItemsList() const
    {
        std::vector<Item *> itemList;
        for (const auto &pair : mItems)
        {
            itemList.push_back(pair.second.get());
        }
        return itemList;
    }
} // namespace ElCalculator::data