#include "InventoryWidget.hpp"

#include <QIcon>

namespace ElCalculator::gui
{

    InventoryWidget::InventoryWidget(QWidget *parent) : QWidget(parent)
    {
        auto *layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        // Initialisation des bonus
        setupButton(data::ItemType::Hint, ":/sprites/hint.png", "Indice (Affiche une aide)");
        setupButton(data::ItemType::Skip, ":/sprites/skip.png", "Passer (Change de question)");
        setupButton(data::ItemType::Solve, ":/sprites/solve.png", "Résoudre (Révèle la réponse)");
        setupButton(data::ItemType::DeleteAnswer, ":/sprites/delete.png", "50/50 (Supprime un faux choix)");

        for (auto const &[type, btn] : mButtons)
        {
            layout->addWidget(btn);
        }
    }

    void InventoryWidget::setupButton(data::ItemType type, const QString& iconPath, const QString& tooltip) {
        auto* btn = new QPushButton(this);

        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(32, 32));
        btn->setText("x0");
        btn->setToolTip(tooltip);
        btn->setEnabled(false);

        // Connexion du signal on click 
        connect(btn, &QPushButton::clicked, [this, type]() {
            emit itemUsed(type);
        });

        mButtons[type] = btn;
    }

    void InventoryWidget::updateInventory(const data::Inventory* inventory)
    {
        if(!inventory) return;

        for(auto* item : inventory->getItemsList()) {
            auto type = item->getId();
            if(mButtons.find(type) != mButtons.end()) {
                int count = item->getCount();
                mButtons[type]->setText("x" + QString::number(count));
                mButtons[type]->setEnabled(count > 0);
            }
        }
    }
}