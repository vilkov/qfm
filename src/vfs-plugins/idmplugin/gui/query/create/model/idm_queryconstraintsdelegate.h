#ifndef IDM_QUERYCONSTRAINTSDELEGATE_H_
#define IDM_QUERYCONSTRAINTSDELEGATE_H_

#include "../../../../model/idm_delegate.h"


IDM_PLUGIN_NS_BEGIN

class QueryConstraintsDelegate : public Delegate
{
	Q_DISABLE_COPY(QueryConstraintsDelegate)

public:
	QueryConstraintsDelegate(QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYCONSTRAINTSDELEGATE_H_ */
