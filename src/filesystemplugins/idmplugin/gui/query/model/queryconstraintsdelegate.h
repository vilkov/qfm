#ifndef QUERYCONSTRAINTSDELEGATE_H_
#define QUERYCONSTRAINTSDELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include "../../../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class QueryConstraintsDelegate : public QStyledItemDelegate
{
	Q_DISABLE_COPY(QueryConstraintsDelegate)

public:
	QueryConstraintsDelegate(QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

IDM_PLUGIN_NS_END

#endif /* QUERYCONSTRAINTSDELEGATE_H_ */
