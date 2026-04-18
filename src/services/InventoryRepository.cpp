#include "InventoryRepository.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

namespace ElCalculator::services
{
    bool InventoryRepository::saveInventory(const data::Inventory &inventory, const QString &filePath)
    {
        QJsonObject root;
        QJsonArray itemsArray;

        for (auto *item : inventory.getItemsList())
        {
            QJsonObject itemObj;
            itemObj["type"] = static_cast<int>(item->getId());
            itemObj["count"] = item->getCount();
            itemsArray.append(itemObj);
        }
        root["items"] = itemsArray;

        QJsonDocument doc(root);
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(doc.toJson());
            file.close();
            return true;
        }
        return false;
    }

    bool InventoryRepository::loadInventory(data::Inventory &inventory, const QString &filePath)
    {
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly))
        {
            return false;
        }

        QByteArray jsonData = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);

        if (doc.isNull() || !doc.isObject())
        {
            return false; 
        }

        QJsonObject root = doc.object();
        QJsonArray itemsArray = root["items"].toArray();

        for (const auto &itemValue : itemsArray)
        {
            QJsonObject itemObj = itemValue.toObject();

            data::ItemType type = static_cast<data::ItemType>(itemObj["type"].toInt());
            int count = itemObj["count"].toInt();

            inventory.addItem(type, count);
        }

        return true;
    }
}