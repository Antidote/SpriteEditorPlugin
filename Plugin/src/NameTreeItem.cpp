#include "NameTreeItem.hpp"
#include "SSprite.hpp"
#include "SSpritePart.hpp"

Q_DECLARE_METATYPE(SSprite*)
Q_DECLARE_METATYPE(SSpritePart*)

NameTreeItem::NameTreeItem(int type)
    : QTreeWidgetItem(type)
{
}

NameTreeItem::NameTreeItem(const QStringList& strings, int type)
    : QTreeWidgetItem(strings, type)
{
}

NameTreeItem::NameTreeItem(QTreeWidget* view, int type)
    : QTreeWidgetItem(view, type)
{
}

NameTreeItem::NameTreeItem(QTreeWidget* view, const QStringList& strings, int type)
    : QTreeWidgetItem(view, strings, type)
{
}

NameTreeItem::NameTreeItem(QTreeWidgetItem* parent, int type)
    : QTreeWidgetItem(parent, type)
{
}

NameTreeItem::NameTreeItem(QTreeWidgetItem* parent, const QStringList& strings, int type)
    : QTreeWidgetItem(parent, strings, type)
{
}

NameTreeItem::NameTreeItem(QTreeWidgetItem* parent, QTreeWidgetItem* after, int type)
    : QTreeWidgetItem(parent, after, type)
{
}

NameTreeItem::NameTreeItem(const QTreeWidgetItem& other)
    : QTreeWidgetItem(other)
{
}

void NameTreeItem::setData(int column, int role, const QVariant& value)
{
    if (column == 1 && role == Qt::EditRole)
    {
        if (data(0, Qt::UserRole).isValid())
        {
            SSprite* sprite = data(0, Qt::UserRole).value<SSprite*>();
            if (sprite)
            {
                sprite->setName(value.toString().toStdString());
                QTreeWidgetItem::setData(column, role, value);
                return;
            }
            SSpritePart* part= data(0, Qt::UserRole).value<SSpritePart*>();
            if (part)
            {
                part->setName(value.toString().toStdString());
                QTreeWidgetItem::setData(column, role, value);
                return;
            }

        }
    }

    QTreeWidgetItem::setData(column, role, value);
}
