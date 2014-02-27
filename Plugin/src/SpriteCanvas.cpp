#include "SpriteCanvas.hpp"
#include <Sprite.hpp>
#include <SpriteFile.hpp>
#include <SpritePart.hpp>
#include <SpriteFrame.hpp>
#include "SpriteEditorPlugin.hpp"
#include "ui_SpriteCanvas.h"
#include <MainWindowBase.hpp>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QWheelEvent>

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
    m_graphicsScene = new QGraphicsScene(this);
    setScene(m_graphicsScene);
    setInteractive(true);
    connect(m_graphicsScene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
    connect(m_graphicsScene, SIGNAL(changed(QList<QRectF>)), this, SLOT(onSelectionChanged()));
}

SpriteCanvas::~SpriteCanvas()
{
    delete m_graphicsScene;
}

void SpriteCanvas::setCurrentSprite(zelda::Sakura::Sprite* cur)
{
    m_currentSprite = cur;
    updateGraphicsScene();
}

zelda::Sakura::Sprite* SpriteCanvas::currentSprite() const
{
    return m_currentSprite;
}

void SpriteCanvas::setCurrentFrame(quint32 curFrame)
{
    m_currentSprite->setCurrentFrame(curFrame);
    updateGraphicsScene();
}

void SpriteCanvas::advanceFrame()
{
    m_currentSprite->advanceFrame();
    updateGraphicsScene();
}

void SpriteCanvas::retreatFrame()
{
    m_currentSprite->retreatFrame();
    updateGraphicsScene();
}

void SpriteCanvas::onSelectionChanged()
{

}

void SpriteCanvas::updateGraphicsScene()
{
    if (!m_currentSprite)
        return;

    MainWindowBase* mw = SpriteEditorPlugin::instance()->mainWindow();
    zelda::Sakura::SpriteFile* spriteFile = m_currentSprite->root();
    if (!m_currentPixmap.load(QString(mw->engineDataPath().absolutePath() + "/" + QString::fromStdString(spriteFile->texture(m_currentSprite->currentState())->Filepath))))
        return;

    m_currentFrame = m_currentSprite->currentFrame();
    m_graphicsScene->clear();
    m_canvasRect = m_graphicsScene->addRect(-m_currentSprite->container()->origin().x(), (-m_currentSprite->container()->origin().y()/2) + 1,
                                            m_currentSprite->container()->size().width(),
                                            m_currentSprite->container()->size().height());

    m_canvasRect->setPen(QPen());
    m_canvasRect->setFlag(QGraphicsItem::ItemIsSelectable);
    m_currentFrame = m_currentSprite->currentFrame();
    foreach (zelda::Sakura::SpritePart* part, m_currentFrame->parts())
    {
        // TODO: Is everything here really needed?
        QPixmap pix = m_currentPixmap.copy(part->textureOffset().x(), part->textureOffset().y(),
                                           part->size().width(),
                                           part->size().height());
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(m_canvasRect);
        pixmapItem->setData(PartKey, QVariant::fromValue<zelda::Sakura::SpritePart*>(part));
        pixmapItem->setTransformOriginPoint(m_currentSprite->container()->origin());
        pixmapItem->setFlag(QGraphicsItem::ItemIsMovable);
        pixmapItem->setFlag(QGraphicsItem::ItemIsSelectable);
        pixmapItem->setFlag(QGraphicsItem::ItemIsPanel);
        pixmapItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        pixmapItem->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        pixmapItem->setAcceptedMouseButtons(Qt::AllButtons);
        pixmapItem->setPixmap(pix);
        pixmapItem->scale(part->flippedHorizontally() ? -1 : 1,
                          part->flippedVertically() ? -1 : 1);
        pixmapItem->translate(part->offset().x(), part->offset().y());

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

