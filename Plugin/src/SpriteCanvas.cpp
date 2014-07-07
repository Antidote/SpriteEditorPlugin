#include "SpriteCanvas.hpp"
#include "PartGraphicsItem.hpp"
#include <Athena/Sprite.hpp>
#include <Athena/SpriteFile.hpp>
#include <Athena/SpritePart.hpp>
#include <Athena/SpriteFrame.hpp>
#include "SpriteEditorPlugin.hpp"
#include "ui_SpriteCanvas.h"
#include <MainWindowBase.hpp>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QWheelEvent>
#include <iostream>
#include <chrono>

inline int floatToMilliseconds(float num)
{
    std::chrono::duration<float> seconds = std::chrono::duration<float>(num);

    return std::chrono::duration_cast<std::chrono::milliseconds>(seconds).count();
}

SpriteCanvas::SpriteCanvas(QWidget* parent)
    : QGraphicsView(parent),
      ui(new Ui::SpriteCanvas),
      m_currentSprite(NULL),
      m_curScaleFactor(1.f),
      m_minScale(.5f),
      m_maxScale(5.f),
      m_isPlaying(false)
{
    ui->setupUi(this);
    m_graphicsScene = new QGraphicsScene(this);
    setScene(m_graphicsScene);
    setInteractive(true);
    connect(&m_spriteTimer, SIGNAL(timeout()), this, SLOT(onSpriteTimeout()));
    m_spriteTimer.start();
}

SpriteCanvas::~SpriteCanvas()
{
    delete m_graphicsScene;
}

void SpriteCanvas::setCurrentSprite(Athena::Sakura::Sprite* cur)
{
    m_currentSprite = cur;
    updateGraphicsScene();
}

Athena::Sakura::Sprite* SpriteCanvas::currentSprite() const
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

void SpriteCanvas::onPlayPause()
{
    if (!m_currentSprite)
        return;

    m_isPlaying ^= 1;
}

void SpriteCanvas::onStop()
{
    if (!m_currentSprite)
        return;

    m_currentSprite->setCurrentFrame((atUint32)0);
    m_isPlaying = false;
}

void SpriteCanvas::onSpriteTimeout()
{
    if (!m_isPlaying || !m_currentSprite)
        return;

    if (m_currentSprite->currentFrame() == m_currentSprite->lastFrame())
    {
        m_currentSprite->setCurrentFrame((atUint32)0);
        updateGraphicsScene();
        return;
    }

    m_currentSprite->advanceFrame();
    updateGraphicsScene();
}

void SpriteCanvas::updateGraphicsScene()
{
    if (!m_currentSprite)
        return;

    MainWindowBase* mw = SpriteEditorPlugin::instance()->mainWindow();
    Athena::Sakura::SpriteFile* spriteFile = m_currentSprite->root();
    if (!m_currentPixmap.load(QString(mw->engineDataPath().absolutePath() + "/" + QString::fromStdString(spriteFile->texture(m_currentSprite->currentState())->Filepath))))
        return;

    m_currentFrame = m_currentSprite->currentFrame();
    m_spriteTimer.setInterval(floatToMilliseconds(m_currentFrame->frameTime()));
    m_graphicsScene->clear();
    QPoint origin = m_currentSprite->container()->origin();
    m_currentFrame = m_currentSprite->currentFrame();
    foreach (Athena::Sakura::SpritePart* part, m_currentFrame->parts())
    {
        // TODO: Is everything here really needed?
        QPixmap pix = m_currentPixmap.copy(part->textureOffset().x(), part->textureOffset().y(),
                                           part->size().width(),
                                           part->size().height());
        PartGraphicsItem* pixmapItem = new PartGraphicsItem(part);
        pixmapItem->setFlag(QGraphicsItem::ItemIsMovable);
        pixmapItem->setFlag(QGraphicsItem::ItemIsSelectable);
        pixmapItem->setFlag(QGraphicsItem::ItemIsPanel);
        pixmapItem->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
        pixmapItem->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
        pixmapItem->setAcceptedMouseButtons(Qt::AllButtons);
        pixmapItem->setPixmap(pix);
        pixmapItem->scale(part->flippedHorizontally() ? -1 : 1, part->flippedVertically() ? -1 : 1);
        pixmapItem->setPos(part->offset().x(), part->offset().y());
        m_graphicsScene->addItem(pixmapItem);
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

bool SpriteCanvas::isPlaying()
{
    return m_isPlaying;
}

void SpriteCanvas::setZoom(int percentZoom)
{
    qreal targetScale = (qreal)percentZoom / 100.0;
    qreal scaleFactor = targetScale / m_curScaleFactor;
    scaleBy(scaleFactor);
}

