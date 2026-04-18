#pragma once

#include "data/Inventory.hpp"

namespace ElCalculator::services
{
    class InventoryRepository
    {
    public:
        static bool saveInventory(const data::Inventory &inventory, const QString &filePath);
        static bool loadInventory(data::Inventory &inventory, const QString &filePath);
    };
} // namespace ElCalculator::services