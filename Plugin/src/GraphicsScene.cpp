#include "GraphicsScene.hpp"
#include <QGraphicsSceneMouseEvent>

GraphicsScene::GraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
}
