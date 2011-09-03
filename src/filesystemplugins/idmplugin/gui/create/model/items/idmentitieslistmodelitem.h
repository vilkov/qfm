#ifndef IDMENTITIESLISTMODELITEM_H_
#define IDMENTITIESLISTMODELITEM_H_

#include "../../../../items/idmentitypropertyitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntitiesListItem : public IdmEntityPropertyItem
{
public:
	IdmEntitiesListItem(IdmEntity *entity, const QString &name, IdmItem *parent = 0) :
		IdmEntityPropertyItem(entity, name, parent)
	{}

	void setEntity(IdmEntity *value) { m_entity = value; }
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITIESLISTMODELITEM_H_ */
