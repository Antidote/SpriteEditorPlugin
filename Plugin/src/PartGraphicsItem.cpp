#include "PartGraphicsItem.hpp"
#include <Athena/SpritePart.hpp>
#include <Athena/SpriteFrame.hpp>
#include <Athena/SpriteFile.hpp>
#include <Athena/Sprite.hpp>

PartGraphicsItem::PartGraphicsItem(Athena::Sakura::SpritePart* part)
    : m_part(part)
{
}


QVariant PartGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange)
    {

        m_part->setOffset(value.toPoint());
        return QPointF(value.toPointF());
    }
    return QGraphicsPixmapItem::itemChange(change, value);
}
