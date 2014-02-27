#ifndef SPRITEFRAME_HPP
#define SPRITEFRAME_HPP

#include <QFrame>
#include <QPixmap>

namespace Ui {
class SpriteEditorFrame;
}

namespace zelda
{
namespace Sakura
{
class SpriteFile;
class Sprite;
class SpriteFrame;
class SpritePart;
}
}

class QTreeWidgetItem;
class ObjectController;

class SpriteEditorFrame : public QFrame
{
    Q_OBJECT

public:
    enum Roles
    {
        SpriteContainerRole= Qt::UserRole + 1,
        SpriteRole,
        PartRole,
        FrameRole
    };

    explicit SpriteEditorFrame(QWidget *parent = 0);
    ~SpriteEditorFrame();

    void setSpriteContainer(zelda::Sakura::SpriteFile* container);

signals:
    void modified();

private slots:
    void onTreeItemChanged(QTreeWidgetItem* item1, QTreeWidgetItem* item2);
    void setFrameOffsetItems(const QString& text, const QVector2D& offset);
    void setFrameSizeItems(const QSize& size);
    void setNameItem(const QString& name, const QVariant& data);
    void onZoomChanged(const QString& zoom);
    void onNameChanged(const QString& name);
private:
    Ui::SpriteEditorFrame *ui;
    QTreeWidgetItem* m_rootItem;
    ObjectController* m_propertyBrowser;
    zelda::Sakura::SpriteFile*  m_spriteContainer;
    zelda::Sakura::Sprite*      m_currentSprite;
    zelda::Sakura::SpriteFrame* m_currentFrame;
    QPixmap m_testPixmap;
};

#endif // SPRITEFRAME_HPP
