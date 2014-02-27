#ifndef GRAPHICSSCENE_HPP
#define GRAPHICSSCENE_HPP

#include <QGraphicsScene>

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = 0);

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:

public slots:

};

#endif // GRAPHICSSCENE_HPP
