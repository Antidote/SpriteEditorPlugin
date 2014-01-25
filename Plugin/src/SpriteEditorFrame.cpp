#include "SpriteEditorFrame.hpp"
#include "ui_SpriteEditorFrame.h"
#include "SSpriteFile.hpp"
#include "SSprite.hpp"
#include "SSpritePart.hpp"
#include "SSpriteFrame.hpp"
#include "NameTreeItem.hpp"
#include <QDebug>
#include <QtTreePropertyBrowser>
#include <QStyledItemDelegate>
#include <QSpinBox>

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

Q_DECLARE_METATYPE(SSpriteFile*)
Q_DECLARE_METATYPE(SSprite*)
Q_DECLARE_METATYPE(SSpritePart*)
Q_DECLARE_METATYPE(SSpriteFrame*)

SpriteEditorFrame::SpriteEditorFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SpriteEditorFrame),
    m_rootItem(NULL),
    m_spriteContainer(NULL)
{
    ui->setupUi(this);
    m_propertyBrowser = new QtTreePropertyBrowser;
    ui->splitter_2->insertWidget(0, m_propertyBrowser);
}

SpriteEditorFrame::~SpriteEditorFrame()
{
    delete ui;
}

void SpriteEditorFrame::setSpriteContainer(SSpriteFile* container)
{
    if (m_spriteContainer != container)
    {
        delete m_spriteContainer;
        m_spriteContainer = container;
        if (m_rootItem)
            delete m_rootItem;
        m_rootItem = new QTreeWidgetItem(QStringList() << "root");
        m_rootItem->setData(0, SpriteContainerRole, QVariant::fromValue<SSpriteFile*>(m_spriteContainer));

        ui->spriteTree->addTopLevelItem(m_rootItem);

        for (std::pair<std::string, SSprite*> sprite : m_spriteContainer->sprites())
        {
            QTreeWidgetItem* spriteItem = new QTreeWidgetItem(QStringList() << QString::fromStdString(sprite.first));
            spriteItem->setData(0, SpriteRole, QVariant::fromValue<SSprite*>(sprite.second));
            m_rootItem->addChild(spriteItem);
            for(SSpritePart* part : sprite.second->parts())
            {
                QTreeWidgetItem* partItem = new QTreeWidgetItem(QStringList() << QString::fromStdString(part->name()));
                partItem->setData(0, PartRole, QVariant::fromValue<SSpritePart*>(part));
                spriteItem->addChild(partItem);
                int frameId = 0;
                for (SSpriteFrame* frame : part->frames())
                {
                    QTreeWidgetItem* frameItem = new QTreeWidgetItem(QStringList() << QString("Frame %1").arg(frameId++));
                    frameItem->setData(0, FrameRole, QVariant::fromValue<SSpriteFrame*>(frame));
                    partItem->addChild(frameItem);
                }
            }
        }
    }
}

void SpriteEditorFrame::onTreeItemChanged(QTreeWidgetItem* item1, QTreeWidgetItem* item2)
{
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
