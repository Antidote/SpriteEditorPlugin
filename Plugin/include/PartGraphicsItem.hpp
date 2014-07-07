#ifndef PARTGRAPHICSITEM_HPP
#define PARTGRAPHICSITEM_HPP

#include <QGraphicsPixmapItem>

namespace Athena
{
namespace Sakura
{
class SpritePart;
}
}

class PartGraphicsItem : public QGraphicsPixmapItem
{
public:
    PartGraphicsItem(Athena::Sakura::SpritePart* part);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
private:
    Athena::Sakura::SpritePart* m_part;
};

#endif // PARTGRAPHICSITEM_HPP
