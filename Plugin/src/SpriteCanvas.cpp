#include "SpriteCanvas.hpp"
#include "SSprite.hpp"
#include "SSpriteFile.hpp"
#include "SSpritePart.hpp"
#include "SSpriteFrame.hpp"
#include "SpriteEditorPlugin.hpp"
#include "ui_SpriteCanvas.h"
#include <MainWindowBase.hpp>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QWheelEvent>

Q_DECLARE_METATYPE(SSpritePart*)

SpriteCanvas::SpriteCanvas(QWidget* parent)
    : QGraphicsView(parent),
      ui(new Ui::SpriteCanvas),
      m_currentSprite(NULL),
      m_canvasRect(NULL),
      m_curScaleFactor(1.f),
      m_minScale(.5f),
      m_maxScale(5.f)
{
    ui->setupUi(this);
    MainWindowBase* mw = SpriteEditorPlugin::instance()->mainWindow();
    m_currentPixmap.load(QString(mw->engineDataPath().absolutePath() + "/sprites/Link/green.png"));
    m_graphicsScene = new QGraphicsScene(this);
    setScene(m_graphicsScene);
    setInteractive(true);
    connect(m_graphicsScene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
}

SpriteCanvas::~SpriteCanvas()
{
    delete m_graphicsScene;
}

void SpriteCanvas::setCurrentSprite(SSprite* cur)
{
    m_currentSprite = cur;
    updateGraphicsScene();
}

SSprite* SpriteCanvas::currentSprite() const
{
    return m_currentSprite;
}

void SpriteCanvas::setCurrentFrame(quint32 curFrame)
{
    m_currentFrame = curFrame;
}

void SpriteCanvas::advanceFrame()
{
    for (SSpritePart* part : m_currentSprite->parts())
        part->advanceFrame();

    updateGraphicsScene();
}

void SpriteCanvas::retreatFrame()
{
    for (SSpritePart* part : m_currentSprite->parts())
        part->retreatFrame();

    updateGraphicsScene();
}

void SpriteCanvas::onSelectionChanged()
{
}

void SpriteCanvas::updateGraphicsScene()
{
    m_graphicsScene->clear();
    m_canvasRect = m_graphicsScene->addRect(-m_currentSprite->container()->originX(), (-m_currentSprite->container()->originY()/2)+2, m_currentSprite->container()->size().width(),
                                                       m_currentSprite->container()->size().height());
    m_canvasRect->setPen(QPen());
    m_canvasRect->setFlag(QGraphicsItem::ItemIsSelectable);
    for (SSpritePart* part : m_currentSprite->parts())
    {
        SSpriteFrame* frame = part->currentFrame();
        QPixmap pix = m_currentPixmap.copy(QRect(frame->textureOffset().toPoint(), frame->size()));
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(m_canvasRect);
        pixmapItem->setData(PartKey, QVariant::fromValue<SSpritePart*>(part));
        pixmapItem->setTransformOriginPoint(m_currentSprite->container()->origin().toPoint());
        pixmapItem->setFlag(QGraphicsItem::ItemIsMovable);
        pixmapItem->setFlag(QGraphicsItem::ItemIsSelectable);
        pixmapItem->setAcceptedMouseButtons(Qt::AllButtons);
        pixmapItem->setPixmap(pix);
        pixmapItem->scale(frame->flippedHorizontally() ? -1 : 1,
                          frame->flippedVertically() ? -1 : 1);
        pixmapItem->translate(frame->offset().x(), frame->offset().y());

    }
}

void SpriteCanvas::scaleBy(double factor)
{
    m_curScaleFactor *= factor;
    scale(factor, factor);
}

void SpriteCanvas::scaleView(qreal scaleFactor)
{
    if (((m_curScaleFactor == m_minScale) && (scaleFactor < 1.0)) ||
        ((m_curScaleFactor == m_maxScale) && (scaleFactor > 1.0))) return;

    qreal sc = scaleFactor;

    if ((m_curScaleFactor * sc < (m_minScale))&&(sc < 1.0))
    {
        sc = m_minScale / m_curScaleFactor;
    }
    else if ((m_curScaleFactor * sc > m_maxScale)&&(sc > 1.0))
    {
        sc = m_maxScale / m_curScaleFactor;
    }
    scaleBy(sc);
}

void SpriteCanvas::setZoom(int percentZoom)
{
    qreal targetScale = (qreal)percentZoom / 100.0;
    qreal scaleFactor = targetScale / m_curScaleFactor;
    scaleBy(scaleFactor);
}

