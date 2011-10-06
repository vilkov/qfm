#ifndef IDMENTITIESLISTMODELITEM_H_
#define IDMENTITIESLISTMODELITEM_H_

#include "../../../../items/idmentitypropertyitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntitiesListItem : public IdmEntityPropertyItem
{
public:
	IdmEntitiesListItem(const QString &name, IdmEntity *entity, IdmItem *parent = 0) :
		IdmEntityPropertyItem(name, entity, parent)
	{}

	void setEntity(IdmEntity *value) { m_entity = value; }
	void setName(const QString &name) { m_name = name; }
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITIESLISTMODELITEM_H_ */
