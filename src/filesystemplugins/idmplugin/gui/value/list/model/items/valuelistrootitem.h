#ifndef VALUELISTROOTITEM_H_
#define VALUELISTROOTITEM_H_

#include "valuelistitem.h"
#include "valuelistvalueitem.h"


IDM_PLUGIN_NS_BEGIN

class ValueListRootItem : public ValueListItem
{
public:
	typedef QList<IdmEntityItem*>  List;
	typedef QMap<IdmEntity*, List> Map;

public:
	ValueListRootItem(IdmItem *parent = 0);

	/* IdmItem */
	virtual bool isEntityItem() const;

	List items(IdmEntity *entity) const { return m_entities.value(entity); }

	void add(IdmEntity *entity, const QString &name)
	{
		ValueListItem *item;

		m_items.push_back(item = new ValueListItem(entity, name, this));
		m_entities[entity].push_back(item);
	}

	void add(IdmItem *item, const QVariant &value)
	{
		static_cast<ValueListItem*>(item)->add(new ValueListValueItem(static_cast<ValueListItem*>(item)->entity(), value, item));
	}

	void remove(IdmEntityItem *item, IdmItemsList::size_type index)
	{
		List &items = m_entities[item->entity()];
		items.removeAt(items.indexOf(item));
		static_cast<ValueListItem*>(item->parent())->remove(index);
	}

private:
	Map m_entities;
};

IDM_PLUGIN_NS_END

#endif /* VALUELISTROOTITEM_H_ */
