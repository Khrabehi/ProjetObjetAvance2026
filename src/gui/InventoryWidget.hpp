#pragma once 

#include <QWidget> 
#include <QPushButton> 
#include <QHBoxLayout>
#include <map> 

#include "data/Inventory.hpp"
#include "data/Item.hpp"

namespace ElCalculator::gui
{
    class InventoryWidget : public QWidget
    {
        Q_OBJECT
    private:
        // Stockage des boutons de l'UI 
        std::map<data::ItemType, QPushButton*> mButtons;
        std::map<data::ItemType, bool> mUsableOverrides;
        std::map<data::ItemType, int> mCounts;

        void setupButton(data::ItemType type, const QString& iconPath, const QString& tooltip);
    public:
        // Explicit est utilisé pour s'assurer que le constructeur ne peut pas être utilisé pour des conversions implicites.
        explicit InventoryWidget(QWidget* parent = nullptr);

        void setItemUsable(data::ItemType type, bool usable);

    public slots:
        // Appelé quand on emit inventoryUpdated
        void updateInventory(const data::Inventory* inventory);

    signals: 
        void itemUsed(data::ItemType type);
    };
} // namespace ElCalculator::gui