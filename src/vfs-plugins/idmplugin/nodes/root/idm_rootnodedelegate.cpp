#include "idm_rootnodedelegate.h"


IDM_PLUGIN_NS_BEGIN

RootNodeDelegate::RootNodeDelegate(const IdmContainer &container, QObject *parent) :
	Delegate(parent),
	m_container(container)
{}

QWidget *RootNodeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return 0;
}

void RootNodeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void RootNodeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

IDM_PLUGIN_NS_END
