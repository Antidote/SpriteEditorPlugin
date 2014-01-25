#ifndef NAMETREEITEM_HPP
#define NAMETREEITEM_HPP

#include <QTreeWidgetItem>

class NameTreeItem : public QTreeWidgetItem
{
public:
    explicit NameTreeItem(int type = Type);
    explicit NameTreeItem(const QStringList &strings, int type = Type);
    explicit NameTreeItem(QTreeWidget *view, int type = Type);
    NameTreeItem(QTreeWidget *view, const QStringList &strings, int type = Type);
    NameTreeItem(QTreeWidget *view, QTreeWidgetItem *after, int type = Type);
    explicit NameTreeItem(QTreeWidgetItem *parent, int type = Type);
    NameTreeItem(QTreeWidgetItem *parent, const QStringList &strings, int type = Type);
    NameTreeItem(QTreeWidgetItem *parent, QTreeWidgetItem *after, int type = Type);
    NameTreeItem(const QTreeWidgetItem &other);
    void setData(int column, int role, const QVariant &value);
};

#endif // NAMETREEITEM_HPP
