#ifndef LISTCONFIGOPTIONDELEGATE_H_
#define LISTCONFIGOPTIONDELEGATE_H_

#include <QtGui/QStyledItemDelegate>


class ListConfigOptionDelegate : public QStyledItemDelegate
{
	Q_DISABLE_COPY(ListConfigOptionDelegate)

public:
	ListConfigOptionDelegate(QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /* LISTCONFIGOPTIONDELEGATE_H_ */
