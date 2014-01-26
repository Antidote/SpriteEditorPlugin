#ifndef SPRITEFRAME_HPP
#define SPRITEFRAME_HPP

#include <QFrame>
#include <QPixmap>

namespace Ui {
class SpriteEditorFrame;
}

class SSpriteFile;
class SSprite;
class SSpritePart;
class SSpriteFrame;
class QTreeWidgetItem;
class QtTreePropertyBrowser;

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

    void setSpriteContainer(SSpriteFile* container);

private slots:
    void onTreeItemChanged(QTreeWidgetItem* item1, QTreeWidgetItem* item2);
    void setFrameOffsetItems(const QString& text, const QVector2D& offset);
    void setFrameSizeItems(const QSize& size);
    void setNameItem(const QString& name, const QVariant& data);
    void onZoomChanged(const QString& zoom);
private:
    Ui::SpriteEditorFrame *ui;
    QTreeWidgetItem* m_rootItem;
    QtTreePropertyBrowser* m_propertyBrowser;
    SSpriteFile* m_spriteContainer;
    QPixmap m_testPixmap;
};

#endif // SPRITEFRAME_HPP
