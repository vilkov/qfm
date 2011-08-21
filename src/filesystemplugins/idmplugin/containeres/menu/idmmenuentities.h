#ifndef IDMMENUENTITIES_H_
#define IDMMENUENTITIES_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include "../../items/idmitemslist.h"
#include "../../items/idmentityitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmMenuEntities : public IdmItemsList
{
public:
	typedef QList<IdmEntityItem*>  List;
	typedef QMap<IdmEntity*, List> Map;

public:
	IdmMenuEntities(const QString &label, const QString &toolTip, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	List items(IdmEntity *entity) const { return m_entities.value(entity); }

	void add(IdmEntity *entity)
	{
		IdmEntityItem *item;

		m_items.push_back(item = new IdmEntityItem(entity, this));
		m_entities[entity].push_back(item);
		expand(item);
	}

	void add(IdmEntityItem *item, IdmEntity *property)
	{
		IdmEntityItem *child;

		item->add(child = new IdmEntityItem(property, item));
		m_entities[property].push_back(child);
		expand(child);
	}

	void remove(IdmEntityItem *item, IdmEntityItem *property)
	{
		List &items = m_entities[property->entity()];
		items.removeAt(items.indexOf(item));
		item->remove(property);
	}

private:
	void expand(IdmEntityItem *parent)
	{
		IdmEntity *entity;
		IdmEntityItem *item;

		for (IdmEntity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
		{
			parent->add(item = new IdmEntityItem(entity = parent->entity()->at(i), parent));
			m_entities[entity].push_back(item);
			expand(item);
		}
	}

private:
	QVariant m_label;
	QVariant m_toolTip;
	Map m_entities;
};

FILE_SYSTEM_NS_END

#endif /* IDMMENUENTITIES_H_ */
