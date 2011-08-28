#ifndef IDMMENUENTITIES_H_
#define IDMMENUENTITIES_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include "idmmenuentitiesitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmMenuEntities : public IdmMenuEntitiesItem
{
public:
	typedef QList<IdmEntityItem*>  List;
	typedef QMap<IdmEntity*, List> Map;

public:
	IdmMenuEntities(const QString &label, const QString &toolTip, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isEntityItem() const;

	List items(IdmEntity *entity) const { return m_entities.value(entity); }

	void add(IdmEntity *entity)
	{
		IdmMenuEntitiesItem *item;

		m_items.push_back(item = new IdmMenuEntitiesItem(entity, this));
		m_entities[entity].push_back(item);
		expand(item);
	}

	void add(IdmEntityItem *item, IdmEntity *property)
	{
		IdmMenuEntitiesItem *child;

		static_cast<IdmMenuEntitiesItem*>(item)->add(child = new IdmMenuEntitiesItem(property, item));
		m_entities[property].push_back(child);
		expand(child);
	}

	void remove(IdmEntityItem *item, IdmItemsList::size_type index)
	{
		List &items = m_entities[item->entity()];
		items.removeAt(items.indexOf(item));
		static_cast<IdmMenuEntitiesItem*>(item->parent())->remove(index);
	}

private:
	void expand(IdmMenuEntitiesItem *parent)
	{
		IdmEntity *entity;
		IdmMenuEntitiesItem *item;

		for (IdmEntity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
		{
			parent->add(item = new IdmMenuEntitiesItem(entity = parent->entity()->at(i), parent));
			m_entities[entity].push_back(item);
			expand(item);
		}
	}

private:
	QVariant m_label;
	QVariant m_toolTip;
	Map m_entities;
};

IDM_PLUGIN_NS_END

#endif /* IDMMENUENTITIES_H_ */
