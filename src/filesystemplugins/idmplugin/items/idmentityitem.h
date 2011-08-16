#ifndef IDMENTITYITEM_H_
#define IDMENTITYITEM_H_

#include "idmitemslist.h"
#include "../storage/entities/idmentity.h"


FILE_SYSTEM_NS_BEGIN

class IdmEntityItem : public IdmItemsList
{
public:
	IdmEntityItem(IdmEntity *entity, IdmItem *parent = 0) :
		IdmItemsList(parent),
		m_entity(entity)
	{
		for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
			m_items.push_back(new IdmEntityItem(entity->at(i), this));
	}

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const
	{
		if (column == 0 && role == Qt::DisplayRole)
			return m_entity->name();
		else
			return QVariant();
	}
	virtual bool isEntityItem() const { return true; }

	void add(IdmEntity *entity) { m_items.push_back(new IdmEntityItem(entity, this)); }

protected:
	IdmEntity *m_entity;
};

FILE_SYSTEM_NS_END

#endif /* IDMENTITYITEM_H_ */
