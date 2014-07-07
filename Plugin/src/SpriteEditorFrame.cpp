#include "SpriteEditorFrame.hpp"
#include "SpriteEditorPlugin.hpp"
#include "ui_SpriteEditorFrame.h"
#include "NameTreeItem.hpp"

#include <QtBrowserItem>
#include <QtProperty>

#include <Athena/SpriteFile.hpp>
#include <Athena/Sprite.hpp>
#include <Athena/SpritePart.hpp>
#include <Athena/SpriteFrame.hpp>

#include <QDebug>
#include "ObjectController.hpp"
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <MainWindowBase.hpp>
#include <QDialog>

class NoEditDelegate: public QStyledItemDelegate
{
public:
    NoEditDelegate(QObject* parent=0): QStyledItemDelegate(parent)
    {}
    virtual QWidget *createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index) const
    {
        Q_UNUSED(parent);
        Q_UNUSED(option);
        Q_UNUSED(index);
        return NULL;
    }
};

SpriteEditorFrame::SpriteEditorFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SpriteEditorFrame),
    m_rootItem(NULL),
    m_spriteContainer(NULL),
    m_currentSprite(NULL),
    m_currentFrame(NULL)
{
    ui->setupUi(this);
    m_propertyBrowser = new ObjectController;
    ui->splitter_2->insertWidget(0, m_propertyBrowser);

    // Cheap way of properly assigning the zoom value
    onZoomChanged();

    connect(ui->advanceFrameButton, SIGNAL(clicked()), ui->spriteCanvas, SLOT(advanceFrame()));
    connect(ui->retreatFrameButton, SIGNAL(clicked()), ui->spriteCanvas, SLOT(retreatFrame()));
    // We have to connect the following two signals in an explicit order
    // The reason is onPlayPause and onStop in spriteCanvas toggles the value acquired
    // from spriteCanvas::isPlaying, and if we connect EditorFrame's signals first, we get the previous value
    // not the current
    connect(ui->playPauseBtn,       SIGNAL(clicked()), ui->spriteCanvas, SLOT(onPlayPause()));
    connect(ui->stopBtn,            SIGNAL(clicked()), ui->spriteCanvas, SLOT(onStop()));
    connect(ui->playPauseBtn,       SIGNAL(clicked()), this, SLOT(onPlayPause()));
    connect(ui->stopBtn,            SIGNAL(clicked()), this, SLOT(onStop()));


    MainWindowBase* mw = SpriteEditorPlugin::instance()->mainWindow();
    if (mw->engineDataPath().exists())
    {
        m_testPixmap.load(mw->engineDataPath().absolutePath() + "/sprites/Link/red.png");
        ui->spriteSheetLabel->setPixmap(m_testPixmap);
    }
}

SpriteEditorFrame::~SpriteEditorFrame()
{
    delete ui;
}

void SpriteEditorFrame::setSpriteContainer(Athena::Sakura::SpriteFile* container)
{
    if (m_spriteContainer != container && container != NULL)
    {
        delete m_spriteContainer;
        m_spriteContainer = container;

        if (m_rootItem)
            delete m_rootItem;
        m_rootItem = new QTreeWidgetItem(QStringList() << "root");
        ui->spriteTree->addTopLevelItem(m_rootItem);

        connect(m_spriteContainer, SIGNAL(modified()), this, SLOT(onModified()));
        connect(m_spriteContainer, SIGNAL(modified()), this, SIGNAL(modified()));
        connect(m_spriteContainer, SIGNAL(originChanged(QPoint)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
        connect(m_spriteContainer, SIGNAL(sizeChanged(QSize)),    ui->spriteCanvas, SLOT(updateGraphicsScene()));

        foreach (Athena::Sakura::Sprite* sprite, m_spriteContainer->sprites())
        {
            QTreeWidgetItem* spriteItem = new QTreeWidgetItem(QStringList() << sprite->name());
            spriteItem->setData(0, SpriteRole, QVariant::fromValue<Athena::Sakura::Sprite*>(sprite));
            connect(sprite, SIGNAL(nameChanged(QString)), this, SLOT(onNameChanged(QString)));
            connect(sprite, SIGNAL(stateChanged(quint32)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
            m_rootItem->addChild(spriteItem);
            int frameId = 0;
            connect(sprite, SIGNAL(frameChanged(SpriteFrame*)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
            foreach(Athena::Sakura::SpriteFrame* frame, sprite->frames())
            {
                QTreeWidgetItem* frameItem = new QTreeWidgetItem(QStringList() << QString("Frame %1").arg(frameId++));
                frameItem->setData(0, FrameRole, QVariant::fromValue<Athena::Sakura::SpriteFrame*>(frame));
                spriteItem->addChild(frameItem);
                connect(frame, SIGNAL(frameTimeChanged(float)), ui->spriteCanvas, SLOT(updateGraphicsScene()));

                foreach (Athena::Sakura::SpritePart* part, frame->parts())
                {
                    // Hook everything up
                    connect(part, SIGNAL(nameChanged(QString)), this, SLOT(onNameChanged(QString)));
                    connect(part, SIGNAL(collisionChanged(bool)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
                    QTreeWidgetItem* partItem = new QTreeWidgetItem(QStringList() << part->name());
                    partItem->setData(0, PartRole, QVariant::fromValue<Athena::Sakura::SpritePart*>(part));
                    connect(part, SIGNAL(orientationChanged(bool,bool)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
                    connect(part, SIGNAL(sizeChanged(QSize)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
                    connect(part, SIGNAL(textureOffsetChanged(QPoint)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
                    frameItem->addChild(partItem);
                }
            }
        }
    }
}

void SpriteEditorFrame::onModified()
{
    QTreeWidgetItem* item = ui->spriteTree->currentItem();
    if (item == m_rootItem)
    {
        m_propertyBrowser->setObject(m_spriteContainer);
    }
    if (item->data(0, SpriteRole).isValid())
    {
        Athena::Sakura::Sprite* sprite = item->data(0, SpriteRole).value<Athena::Sakura::Sprite*>();
        m_propertyBrowser->setObject(sprite);
    }
    if (item->data(0, FrameRole).isValid())
    {
        Athena::Sakura::SpriteFrame* frame = item->data(0, FrameRole).value<Athena::Sakura::SpriteFrame*>();
        m_propertyBrowser->setObject(frame);
    }
    if (item->data(0, PartRole).isValid())
    {
        Athena::Sakura::SpritePart* part = item->data(0, PartRole).value<Athena::Sakura::SpritePart*>();
        m_propertyBrowser->setObject(part);
    }
    m_propertyBrowser->update();
}

void SpriteEditorFrame::onTreeItemChanged(QTreeWidgetItem* item1, QTreeWidgetItem* item2)
{
    Q_UNUSED(item2);

    if (item1 == m_rootItem)
    {
        m_propertyBrowser->setObject(m_spriteContainer);
    }
    if (item1->data(0, SpriteRole).isValid())
    {
        Athena::Sakura::Sprite* sprite = item1->data(0, SpriteRole).value<Athena::Sakura::Sprite*>();
        m_currentSprite = sprite;
        ui->spriteCanvas->setCurrentSprite(sprite);
        m_propertyBrowser->setObject(sprite);
    }
    if (item1->data(0, FrameRole).isValid())
    {
        Athena::Sakura::SpriteFrame* frame = item1->data(0, FrameRole).value<Athena::Sakura::SpriteFrame*>();
        m_propertyBrowser->setObject(frame);
        QString frameStr = item1->text(0).remove("Frame ");
        bool ok = false;
        int frameIndex = frameStr.toInt(&ok);
        m_currentFrame = frame;
        m_currentSprite = frame->root();
        ui->spriteCanvas->setCurrentSprite(m_currentSprite);

        if (ok && m_currentSprite)
        {
            qDebug() << frameIndex;
            ui->spriteCanvas->setCurrentFrame(frameIndex);
        }
    }
    if (item1->data(0, PartRole).isValid())
    {
        Athena::Sakura::SpritePart* part = item1->data(0, PartRole).value<Athena::Sakura::SpritePart*>();
        m_propertyBrowser->setObject(part);
        m_currentSprite = part->root()->root();
        m_currentSprite->setCurrentFrame(part->root());
        m_currentFrame = m_currentSprite->currentFrame();
        ui->spriteCanvas->setCurrentSprite(m_currentSprite);
    }
}

void SpriteEditorFrame::setFrameOffsetItems(const QString& text, const QVector2D& offset)
{
}

void SpriteEditorFrame::setFrameSizeItems(const QSize& size)
{
}

void SpriteEditorFrame::setNameItem(const QString& name, const QVariant& data)
{
}

void SpriteEditorFrame::onZoomChanged()
{
    if (!updatesEnabled())
        return;
    setUpdatesEnabled(false);
    if (sender() == ui->zoomSpinBox)
        ui->zoomSlider->setValue(ui->zoomSpinBox->value());
    else
        ui->zoomSpinBox->setValue(ui->zoomSlider->value());

    ui->spriteCanvas->setZoom(ui->zoomSlider->value());
    setUpdatesEnabled(true);
}

void SpriteEditorFrame::onNameChanged(const QString& name)
{
    Athena::Sakura::Sprite* sprite = qobject_cast<Athena::Sakura::Sprite*>(sender());
    Athena::Sakura::SpritePart* part = qobject_cast<Athena::Sakura::SpritePart*>(sender());
    if (sprite || part)
    {
        QTreeWidgetItemIterator it(ui->spriteTree);
        while (*it)
        {
            if (((sprite && sprite == (*it)->data(0, SpriteRole).value<Athena::Sakura::Sprite*>())
                 || (part && part == (*it)->data(0, PartRole).value<Athena::Sakura::SpritePart*>()))
                && (*it)->text(0).compare(name))
            {
                (*it)->setText(0, name);
                emit modified();
            }
            ++it;
        }
    }
}

void SpriteEditorFrame::onPlayPause()
{
    if (ui->spriteCanvas->isPlaying())
        ui->playPauseBtn->setIcon(QIcon::fromTheme("media-playback-pause"));
    else
        ui->playPauseBtn->setIcon(QIcon::fromTheme("media-playback-start"));
}

void SpriteEditorFrame::onStop()
{
    ui->playPauseBtn->setIcon(QIcon::fromTheme("media-playback-start"));
}
