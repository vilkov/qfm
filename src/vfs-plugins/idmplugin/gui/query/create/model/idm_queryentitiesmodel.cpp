#include "idm_queryentitiesmodel.h"
#include "items/idm_queryentitiesmodelitem.h"


IDM_PLUGIN_NS_BEGIN

QueryEntitiesModel::QueryEntitiesModel(QObject *parent) :
	IdmModel(parent)
{}

void QueryEntitiesModel::add(const IdmEntity::Property &property)
{
	QueryEntitiesModelItem *item;

	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(item = new QueryEntitiesModelItem(property));
	expand(item);
	endInsertRows();
}

void QueryEntitiesModel::remove(const QModelIndex &index)
{
//	beginRemoveRows(QModelIndex(), index.row(), index.row());
//	delete m_items.at(index.row());
//	m_items.remove(index.row());
//	endRemoveRows();
}

void QueryEntitiesModel::expand(IdmItem *p)
{
	QueryEntitiesModelItem *item;
	QueryEntitiesModelItem *parent = static_cast<QueryEntitiesModelItem*>(p);

	for (IdmEntity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
	{
		parent->add(item = new QueryEntitiesModelItem(parent->entity()->at(i), parent));
		expand(item);
	}
}

IDM_PLUGIN_NS_END
