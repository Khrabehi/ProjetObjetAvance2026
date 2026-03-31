#pragma once

#include <QString>
#include <string>

namespace NomCool::data
{
    enum class ItemType
    {
        Hint,
        Skip,
        Solve,
        DeleteAnswer
    };

    class Item
    {
    public:
        Item(ItemType id, const std::string &name, const QString &sprite)
            : mId(id), mName(name), mCount(0), mSprite(sprite)
        {
        }

        virtual ~Item() = default;

        virtual void use() = 0;
        virtual std::string describe() const = 0;
        virtual QString getIcon() const = 0;

        int getCount() const { return mCount; }
        void addAmount(int amount)
        {
            mCount += amount;
        }

        bool removeAmount(int amount)
        {
            if (mCount >= amount)
            {
                mCount -= amount;
                return true;
            }
            return false;
        }

    protected:
        ItemType mId;
        std::string mName;
        int mCount;
        QString mSprite;
    };
}// namespace NomCool::data