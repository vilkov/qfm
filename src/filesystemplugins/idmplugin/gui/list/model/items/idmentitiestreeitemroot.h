#ifndef IDMENTITIESTREEITEMROOT_H_
#define IDMENTITIESTREEITEMROOT_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include "idmentitiestreeitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntitiesTreeItemRoot : public IdmEntitiesTreeItem
{
public:
	typedef QList<IdmEntityItem*>  List;
	typedef QMap<IdmEntity*, List> Map;

public:
	IdmEntitiesTreeItemRoot(IdmItem *parent = 0);

	/* IdmItem */
	virtual bool isEntityItem() const;

	List items(IdmEntity *entity) const { return m_entities.value(entity); }

	void add(IdmEntity *entity)
	{
		IdmEntitiesTreeItem *item;

		m_items.push_back(item = new IdmEntitiesTreeItem(entity, this));
		m_entities[entity].push_back(item);
		expand(item);
	}

	void add(IdmEntityItem *item, IdmEntity *property)
	{
		IdmEntitiesTreeItem *child;

		static_cast<IdmEntitiesTreeItem*>(item)->add(child = new IdmEntitiesTreeItem(property, item));
		m_entities[property].push_back(child);
		expand(child);
	}

	void remove(IdmEntityItem *item, IdmItemsList::size_type index)
	{
		List &items = m_entities[item->entity()];
		items.removeAt(items.indexOf(item));
		static_cast<IdmEntitiesTreeItem*>(item->parent())->remove(index);
	}

private:
	void expand(IdmEntitiesTreeItem *parent)
	{
		IdmEntity *entity;
		IdmEntitiesTreeItem *item;

		for (IdmEntity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
		{
			parent->add(item = new IdmEntitiesTreeItem(entity = parent->entity()->at(i), parent));
			m_entities[entity].push_back(item);
			expand(item);
		}
	}

private:
	Map m_entities;
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITIESTREEITEMROOT_H_ */
