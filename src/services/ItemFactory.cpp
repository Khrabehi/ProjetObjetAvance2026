#include "ItemFactory.hpp"
#include "items/HintItem.hpp"
#include "items/SkipItem.hpp"
#include "items/SolveItem.hpp"
#include "items/DeleteAnswerItem.hpp"

#include <random>

namespace ElCalculator::services
{
    std::unique_ptr<data::Item> ItemFactory::createItem(data::ItemType type)
    {
        switch (type)
        {
        case data::ItemType::Hint:
            return std::make_unique<items::HintItem>();
        case data::ItemType::Skip:
            return std::make_unique<items::SkipItem>();
        case data::ItemType::Solve:
            return std::make_unique<items::SolveItem>();
        case data::ItemType::DeleteAnswer:
            return std::make_unique<items::DeleteAnswerItem>();
        default:
            return nullptr;
        }
    }

    std::unique_ptr<data::Item> ItemFactory::createRandomItem()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::vector<data::ItemType> types = {
            data::ItemType::Hint,
            data::ItemType::Skip,
            data::ItemType::Solve,
            data::ItemType::DeleteAnswer};

        std::uniform_int_distribution<> dis(0, types.size() - 1);
        return createItem(types[dis(gen)]);
    }
}
