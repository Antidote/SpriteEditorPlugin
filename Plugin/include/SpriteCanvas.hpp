#ifndef SPRITECANVAS_HPP
#define SPRITECANVAS_HPP

#include <QGraphicsView>
#include <QPixmap>
#include <QTimer>

namespace Ui {
class SpriteCanvas;
}

namespace Athena
{
namespace Sakura
{
class Sprite;
class SpriteFrame;
}
}
class QPaintEvent;
class QGraphicsScene;
class QWheelEvent;
class SpriteCanvas : public QGraphicsView
{
    Q_OBJECT

public:
    SpriteCanvas(QWidget* parent = 0);
    ~SpriteCanvas();

    void setCurrentSprite(Athena::Sakura::Sprite* cur);
    Athena::Sakura::Sprite* currentSprite() const;

    void setCurrentFrame(quint32 curFrame);

    void scaleBy(qreal factor);
    void scaleView(qreal scaleFactor);

    bool isPlaying();
public slots:
    void setZoom(int percentZoom);
    void advanceFrame();
    void retreatFrame();
protected:
private slots:
    void onPlayPause();
    void onStop();
    void onSpriteTimeout();
    void updateGraphicsScene();

private:
    enum {PartKey = 65535};

    Ui::SpriteCanvas* ui;
    Athena::Sakura::Sprite*      m_currentSprite;
    Athena::Sakura::SpriteFrame* m_currentFrame;
    QPixmap                      m_currentPixmap;
    QGraphicsScene*              m_graphicsScene;
    bool                         m_isPlaying;
    qreal                        m_curScaleFactor;
    qreal                        m_minScale;
    qreal                        m_maxScale;
    QTimer                       m_spriteTimer;
};

#endif // SPRITECANVAS_HPP
