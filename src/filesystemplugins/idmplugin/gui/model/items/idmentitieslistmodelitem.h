#ifndef IDMENTITIESLISTMODELITEM_H_
#define IDMENTITIESLISTMODELITEM_H_

#include "../../../items/idmentityitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmEntitiesListItem : public IdmEntityItem
{
public:
	IdmEntitiesListItem(IdmEntity *entity, IdmItem *parent = 0) :
		IdmEntityItem(entity, parent)
	{}

	void setEntity(IdmEntity *value) { m_entity = value; }
};

FILE_SYSTEM_NS_END

#endif /* IDMENTITIESLISTMODELITEM_H_ */
