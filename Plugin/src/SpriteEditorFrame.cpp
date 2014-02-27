#include "SpriteEditorFrame.hpp"
#include "SpriteEditorPlugin.hpp"
#include "ui_SpriteEditorFrame.h"
#include "NameTreeItem.hpp"

#include <QtBrowserItem>
#include <QtProperty>

#include <SpriteFile.hpp>
#include <Sprite.hpp>
#include <SpritePart.hpp>
#include <SpriteFrame.hpp>

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

    connect(ui->advanceFrameButton, SIGNAL(clicked()), ui->spriteCanvas, SLOT(advanceFrame()));
    connect(ui->retreatFrameButton, SIGNAL(clicked()), ui->spriteCanvas, SLOT(retreatFrame()));


    MainWindowBase* mw = SpriteEditorPlugin::instance()->mainWindow();
    if (mw->engineDataPath().exists())
    {
        //m_testPixmap.load(mw->engineDataPath().absolutePath() + "/sprites/Link/red.png");
        //ui->spriteSheetLabel->setPixmap(m_testPixmap);
    }
}

SpriteEditorFrame::~SpriteEditorFrame()
{
    delete ui;
}

void SpriteEditorFrame::setSpriteContainer(zelda::Sakura::SpriteFile* container)
{
    if (m_spriteContainer != container && container != NULL)
    {
        delete m_spriteContainer;
        m_spriteContainer = container;

        if (m_rootItem)
            delete m_rootItem;
        m_rootItem = new QTreeWidgetItem(QStringList() << "root");
        ui->spriteTree->addTopLevelItem(m_rootItem);

        connect(m_spriteContainer, SIGNAL(originChanged(QPoint)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
        connect(m_spriteContainer, SIGNAL(sizeChanged(QSize)),    ui->spriteCanvas, SLOT(updateGraphicsScene()));

        foreach (zelda::Sakura::Sprite* sprite, m_spriteContainer->sprites())
        {
            QTreeWidgetItem* spriteItem = new QTreeWidgetItem(QStringList() << sprite->name());
            spriteItem->setData(0, SpriteRole, QVariant::fromValue<zelda::Sakura::Sprite*>(sprite));
            connect(sprite, SIGNAL(nameChanged(QString)), this, SLOT(onNameChanged(QString)));
            connect(sprite, SIGNAL(stateChanged(quint32)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
            m_rootItem->addChild(spriteItem);
            int frameId = 0;
            connect(sprite, SIGNAL(frameChanged(SpriteFrame*)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
            foreach(zelda::Sakura::SpriteFrame* frame, sprite->frames())
            {
                QTreeWidgetItem* frameItem = new QTreeWidgetItem(QStringList() << QString("Frame %1").arg(frameId++));
                frameItem->setData(0, FrameRole, QVariant::fromValue<zelda::Sakura::SpriteFrame*>(frame));
                spriteItem->addChild(frameItem);
                connect(frame, SIGNAL(frameTimeChanged(float)), ui->spriteCanvas, SLOT(updateGraphicsScene()));

                foreach (zelda::Sakura::SpritePart* part, frame->parts())
                {
                    // Hook everything up
                    connect(part, SIGNAL(nameChanged(QString)), this, SLOT(onNameChanged(QString)));
                    connect(part, SIGNAL(collisionChanged(bool)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
                    QTreeWidgetItem* partItem = new QTreeWidgetItem(QStringList() << part->name());
                    partItem->setData(0, PartRole, QVariant::fromValue<zelda::Sakura::SpritePart*>(part));
                    connect(part, SIGNAL(orientationChanged(bool,bool)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
                    connect(part, SIGNAL(offsetChanged(QPoint)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
                    connect(part, SIGNAL(sizeChanged(QSize)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
                    connect(part, SIGNAL(textureOffsetChanged(QPoint)), ui->spriteCanvas, SLOT(updateGraphicsScene()));
                    frameItem->addChild(partItem);
                }
            }
        }
    }
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
        zelda::Sakura::Sprite* sprite = item1->data(0, SpriteRole).value<zelda::Sakura::Sprite*>();
        m_currentSprite = sprite;
        ui->spriteCanvas->setCurrentSprite(sprite);
        m_propertyBrowser->setObject(sprite);
    }
    if (item1->data(0, FrameRole).isValid())
    {
        zelda::Sakura::SpriteFrame* frame = item1->data(0, FrameRole).value<zelda::Sakura::SpriteFrame*>();
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
        zelda::Sakura::SpritePart* part = item1->data(0, PartRole).value<zelda::Sakura::SpritePart*>();
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

void SpriteEditorFrame::onZoomChanged(const QString& zoom)
{
    QString value = zoom;
    value = value.remove('%');
    bool ok = false;

    qreal zoomFactor = value.toInt(&ok);
    if (ok)
        ui->spriteCanvas->setZoom(zoomFactor);
}

void SpriteEditorFrame::onNameChanged(const QString& name)
{
    zelda::Sakura::Sprite* sprite = qobject_cast<zelda::Sakura::Sprite*>(sender());
    zelda::Sakura::SpritePart* part = qobject_cast<zelda::Sakura::SpritePart*>(sender());
    if (sprite || part)
    {
        QTreeWidgetItemIterator it(ui->spriteTree);
        while (*it)
        {
            if (((sprite && sprite == (*it)->data(0, SpriteRole).value<zelda::Sakura::Sprite*>())
                 || (part && part == (*it)->data(0, PartRole).value<zelda::Sakura::SpritePart*>()))
                && (*it)->text(0).compare(name))
            {
                (*it)->setText(0, name);
                emit modified();
            }
            ++it;
        }
    }
}
