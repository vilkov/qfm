#ifndef IDM_ROOTNODEDELEGATE_H_
#define IDM_ROOTNODEDELEGATE_H_

#include "../../model/idm_delegate.h"
#include "../../containeres/idm_container.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeDelegate : public Delegate
{
	Q_DISABLE_COPY(RootNodeDelegate)

public:
	RootNodeDelegate(const IdmContainer &container, QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    mutable IdmContainer m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ROOTNODEDELEGATE_H_ */
