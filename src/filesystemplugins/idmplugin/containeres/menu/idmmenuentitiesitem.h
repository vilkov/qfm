#ifndef IDMMENUENTITIESITEM_H_
#define IDMMENUENTITIESITEM_H_

#include "../../items/idmentityitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmMenuEntitiesItem : public IdmEntityItem
{
public:
	IdmMenuEntitiesItem(IdmEntity *entity, IdmItem *parent = 0);

protected:
	friend class IdmMenuEntities;
	void add(IdmEntityItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }
};

FILE_SYSTEM_NS_END

#endif /* IDMMENUENTITIESITEM_H_ */
