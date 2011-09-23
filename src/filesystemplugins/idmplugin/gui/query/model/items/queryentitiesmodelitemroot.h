#ifndef QUERYENTITIESMODELITEMROOT_H_
#define QUERYENTITIESMODELITEMROOT_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include "queryentitiesmodelitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryEntitiesModelItemRoot : public QueryEntitiesModelItem
{
public:
	typedef QList<IdmEntityItem*>  List;
	typedef QMap<IdmEntity*, List> Map;

public:
	QueryEntitiesModelItemRoot(IdmItem *parent = 0);

	/* IdmItem */
	virtual bool isEntityItem() const;

	List items(IdmEntity *entity) const { return m_entities.value(entity); }

	void add(const QString &name, IdmEntity *entity)
	{
		QueryEntitiesModelItem *item;

		m_items.push_back(item = new QueryEntitiesModelItem(name, entity, this));
		m_entities[entity].push_back(item);
		expand(item);
	}

	void remove(IdmEntityItem *item, IdmItemsList::size_type index)
	{
		List &items = m_entities[item->entity()];
		items.removeAt(items.indexOf(item));
		static_cast<QueryEntitiesModelItem*>(item->parent())->remove(index);
	}

private:
	void expand(QueryEntitiesModelItem *parent)
	{
		IdmEntity *entity;
		QueryEntitiesModelItem *item;

		for (IdmEntity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
		{
			parent->add(item = new QueryEntitiesModelItem(parent->entity()->at(i).name, entity = parent->entity()->at(i).entity, parent));
			m_entities[entity].push_back(item);
			expand(item);
		}
	}

private:
	Map m_entities;
};

IDM_PLUGIN_NS_END

#endif /* QUERYENTITIESMODELITEMROOT_H_ */
