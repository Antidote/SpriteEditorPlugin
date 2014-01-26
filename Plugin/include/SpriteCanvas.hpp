#ifndef SPRITECANVAS_HPP
#define SPRITECANVAS_HPP

#include <QGraphicsView>
#include <QPixmap>

namespace Ui {
class SpriteCanvas;
}

class SSprite;
class QPaintEvent;
class QGraphicsScene;
class QWheelEvent;
class SpriteCanvas : public QGraphicsView
{
    Q_OBJECT

public:
    SpriteCanvas(QWidget* parent = 0);
    ~SpriteCanvas();

    void setCurrentSprite(SSprite* cur);
    SSprite* currentSprite() const;

    void setCurrentFrame(quint32 curFrame);

    void scaleBy(double factor);
    void scaleView(qreal scaleFactor);
public slots:
    void setZoom(int percentZoom);
    void advanceFrame();
    void retreatFrame();
protected:
private slots:
    void onSelectionChanged();
    void updateGraphicsScene();

private:
    enum {PartKey = 65535};

    Ui::SpriteCanvas* ui;
    SSprite*   m_currentSprite;
    quint32    m_currentFrame;
    QPixmap    m_currentPixmap;
    QGraphicsScene* m_graphicsScene;
    QGraphicsRectItem* m_canvasRect;
    qreal      m_curScaleFactor;
    qreal      m_minScale;
    qreal      m_maxScale;
};

#endif // SPRITECANVAS_HPP
