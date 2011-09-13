#ifndef VALUESROOTTREEITEM_H_
#define VALUESROOTTREEITEM_H_

#include "valuestreeitem.h"


IDM_PLUGIN_NS_BEGIN

class ValuesRootTreeItem : public ValuesTreeItem
{
public:
	typedef QList<IdmEntityItem*>  List;
	typedef QMap<IdmEntity*, List> Map;

public:
	ValuesRootTreeItem(IdmItem *parent = 0);

	/* IdmItem */
	virtual bool isEntityItem() const;

	List items(IdmEntity *entity) const { return m_entities.value(entity); }

	void add(IdmEntity *entity)
	{
		ValuesTreeItem *item;

		m_items.push_back(item = new ValuesTreeItem(entity, this));
		m_entities[entity].push_back(item);
		expand(item);
	}

	void add(IdmEntityItem *item, IdmEntity *property)
	{
		ValuesTreeItem *child;

		static_cast<ValuesTreeItem*>(item)->add(child = new ValuesTreeItem(property, item));
		m_entities[property].push_back(child);
		expand(child);
	}

	void remove(IdmEntityItem *item, IdmItemsList::size_type index)
	{
		List &items = m_entities[item->entity()];
		items.removeAt(items.indexOf(item));
		static_cast<ValuesTreeItem*>(item->parent())->remove(index);
	}

private:
	void expand(ValuesTreeItem *parent)
	{
		IdmEntity *entity;
		ValuesTreeItem *item;

		for (IdmEntity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
		{
			parent->add(item = new ValuesTreeItem(entity = parent->entity()->at(i), parent));
			m_entities[entity].push_back(item);
			expand(item);
		}
	}

private:
	Map m_entities;
};

IDM_PLUGIN_NS_END

#endif /* VALUESROOTTREEITEM_H_ */
