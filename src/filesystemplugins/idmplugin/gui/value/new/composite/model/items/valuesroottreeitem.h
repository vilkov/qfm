#ifndef VALUESROOTTREEITEM_H_
#define VALUESROOTTREEITEM_H_

#include "valuestreeitem.h"
#include "valuestreevalueitem.h"


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

	void add(IdmEntity *entity, const QString &name)
	{
		ValuesTreeItem *item;

		m_items.push_back(item = new ValuesTreeItem(entity, name, this));
		m_entities[entity].push_back(item);
	}

	void add(IdmItem *item, IdmEntityValue *value)
	{
		static_cast<ValuesTreeItem*>(item)->add(new ValuesTreeValueItem(static_cast<ValuesTreeItem*>(item)->entity(), value, item));
	}

	void remove(IdmEntityItem *item, IdmItemsList::size_type index)
	{
		List &items = m_entities[item->entity()];
		items.removeAt(items.indexOf(item));
		static_cast<ValuesTreeItem*>(item->parent())->remove(index);
	}

private:
	Map m_entities;
};

IDM_PLUGIN_NS_END

#endif /* VALUESROOTTREEITEM_H_ */
