#include "idm_entitieslistmodel.h"
#include "items/idm_entitieslistmodelitem.h"


IDM_PLUGIN_NS_BEGIN

EntitiesListModel::EntitiesListModel(QObject *parent) :
	Model(parent)
{}

int EntitiesListModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

Qt::ItemFlags EntitiesListModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

IdmEntity *EntitiesListModel::entityAt(size_type index) const
{
	return static_cast<IdmEntitiesListItem*>(m_items.at(index))->entity();
}

const QString &EntitiesListModel::nameAt(size_type index) const
{
	return static_cast<IdmEntitiesListItem*>(m_items.at(index))->property().name;
}

void EntitiesListModel::add(IdmEntity *entity, const QString &name)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(new IdmEntitiesListItem(IdmEntity::Property(entity, name)));
	endInsertRows();
}

void EntitiesListModel::remove(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	delete m_items.takeAt(index.row());
	endRemoveRows();
}

IDM_PLUGIN_NS_END
