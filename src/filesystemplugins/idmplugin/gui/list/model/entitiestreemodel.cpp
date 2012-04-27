#include "entitiestreemodel.h"
#include "items/idmentitiestreeitem.h"


IDM_PLUGIN_NS_BEGIN

EntitiesTreeModel::EntitiesTreeModel(QObject *parent) :
	IdmModel(parent)
{}

void EntitiesTreeModel::add(IdmEntity *entity)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	doAdd(entity);
	endInsertRows();
}

void EntitiesTreeModel::remove(const QModelIndex &index)
{
//	beginRemoveRows(QModelIndex(), index.row(), index.row());
//	delete m_items.at(index.row());
//	m_items.remove(index.row());
//	endRemoveRows();
}

void EntitiesTreeModel::doAdd(IdmEntity *entity)
{
	IdmEntitiesTreeItem *item;

	m_items.push_back(item = new IdmEntitiesTreeItem(entity));
	m_entities[entity].push_back(item);
	expand(item);
}

void EntitiesTreeModel::doAdd(IdmItem *item, IdmEntity *property)
{
	IdmEntitiesTreeItem *child;

	static_cast<IdmEntitiesTreeItem*>(item)->add(child = new IdmEntitiesTreeItem(property, item));
	m_entities[property].push_back(child);
	expand(child);
}

void EntitiesTreeModel::doRemove(IdmItem *item, Container::size_type index)
{
	Container::List &items = m_entities[static_cast<IdmEntitiesTreeItem *>(item)->entity()];
	items.removeAt(items.indexOf(item));
	static_cast<IdmEntitiesTreeItem*>(item->parent())->remove(index);
}

void EntitiesTreeModel::expand(IdmItem *p)
{
	IdmEntity *entity;
	IdmEntitiesTreeItem *item;
	IdmEntitiesTreeItem *parent = static_cast<IdmEntitiesTreeItem *>(p);

	for (IdmEntity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
	{
		parent->add(item = new IdmEntitiesTreeItem(entity = parent->entity()->at(i).entity, parent));
		m_entities[entity].push_back(item);
		expand(item);
	}
}

IDM_PLUGIN_NS_END
