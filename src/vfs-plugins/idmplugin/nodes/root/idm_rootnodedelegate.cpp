#include "idm_rootnodedelegate.h"


IDM_PLUGIN_NS_BEGIN

IdmRootNodeDelegate::IdmRootNodeDelegate(const IdmContainer &container, QObject *parent) :
	IdmDelegate(parent),
	m_container(container)
{}

QWidget *IdmRootNodeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return 0;
}

void IdmRootNodeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void IdmRootNodeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

IDM_PLUGIN_NS_END
