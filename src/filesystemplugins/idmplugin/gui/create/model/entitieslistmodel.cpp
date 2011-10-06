#include "entitieslistmodel.h"
#include "items/idmentitieslistmodelitem.h"
#include "../../../items/idmentityitem.h"


IDM_PLUGIN_NS_BEGIN

EntitiesListModel::EntitiesListModel(QObject *parent) :
	IdmModel(parent)
{}

int EntitiesListModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

IdmEntity *EntitiesListModel::entityAt(size_type index) const
{
	return static_cast<IdmEntitiesListItem*>(m_items.at(index))->entity();
}

QString EntitiesListModel::nameAt(size_type index) const
{
	return static_cast<IdmEntitiesListItem*>(m_items.at(index))->name().toString();
}

void EntitiesListModel::add(IdmEntity *entity, const QString &name)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(new IdmEntitiesListItem(name, entity));
	endInsertRows();
}

void EntitiesListModel::remove(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	delete m_items.takeAt(index.row());
	endRemoveRows();
}

IDM_PLUGIN_NS_END
