#ifndef IDMENTITIESLISTMODELITEM_H_
#define IDMENTITIESLISTMODELITEM_H_

#include "../../../../items/idmentitypropertyitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntitiesListItem : public IdmEntityPropertyItem
{
public:
	IdmEntitiesListItem(const IdmEntity::Property &property, IdmItem *parent = 0) :
		IdmEntityPropertyItem(property, parent)
	{}

	void setEntity(IdmEntity *value) { m_property.entity = m_entity = value; }
	void setName(const QString &name) { m_property.name = name; }
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITIESLISTMODELITEM_H_ */
