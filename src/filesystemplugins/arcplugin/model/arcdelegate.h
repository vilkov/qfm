#ifndef ARCDELEGATE_H_
#define ARCDELEGATE_H_

#include "../arcplugin_ns.h"
#include "../../../filesystem/tasks/filesystemtasksnodedelegate.h"


ARC_PLUGIN_NS_BEGIN

class ArcDelegate : public TasksNodeDelegate
{
	Q_DISABLE_COPY(ArcDelegate)

public:
	ArcDelegate(QAbstractProxyModel *proxy, QObject *parent = 0);

    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

ARC_PLUGIN_NS_END

#endif /* ARCDELEGATE_H_ */
