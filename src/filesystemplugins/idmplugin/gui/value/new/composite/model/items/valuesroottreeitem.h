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
	typedef QList<IdmEntityValue*> ValueList;

public:
	ValuesRootTreeItem(IdmItem *parent = 0);

	/* IdmItem */
	virtual bool isEntityItem() const;

	const Map &entities() const { return m_entities; }

	void add(IdmEntity *entity, const QString &name)
	{
		ValuesTreeItem *item;

		m_items.push_back(item = new ValuesTreeItem(entity, name, this));
		m_entities[entity].push_back(item);
	}

	void add(IdmItem *item, const ValueList &values)
	{
		for (ValueList::size_type i = 0, size = values.size(); i < size; ++i)
			static_cast<ValuesTreeItem*>(item)->add(new ValuesTreeValueItem(static_cast<ValuesTreeItem*>(item)->entity(), values.at(i), item));
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
