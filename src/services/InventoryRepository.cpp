#include "InventoryRepository.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

namespace NomCool::services {
    bool InventoryRepository::saveInventory(const data::Inventory &inventory, const QString &filePath) {
        QJsonObject root;
        QJsonArray itemsArray;

        for(auto* item : inventory.getItemsList()) {
            QJsonObject itemObj;
            itemObj["type"] = static_cast<int>(item->getId());
            itemObj["count"] = item->getCount();
            itemsArray.append(itemObj);
        }
        root["items"] = itemsArray;

        QJsonDocument doc(root);
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(doc.toJson());
            file.close();
            return true;
        }
        return false;
    }

    bool InventoryRepository::loadInventory(data::Inventory &inventory, const QString &filePath) {
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly)) {
            return false;
        }

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonArray itemsArray = doc.object()["items"].toArray();

        for (const auto &itemVal : itemsArray) {
            QJsonObject itemObj = itemVal.toObject();
            data::ItemType type = static_cast<data::ItemType>(itemObj["type"].toInt());
            int count = itemObj["count"].toInt();
            inventory.addItem(type, count);
        }
        return true;
    }
}