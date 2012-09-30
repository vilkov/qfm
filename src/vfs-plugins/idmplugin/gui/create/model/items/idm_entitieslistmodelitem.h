#ifndef IDM_ENTITIESLISTMODELITEM_H_
#define IDM_ENTITIESLISTMODELITEM_H_

#include "../../../../model/items/idm_listitem.h"
#include "../../../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntitiesListItem : public IdmListItem
{
public:
	IdmEntitiesListItem(const Entity::Property &property, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	Entity *entity() const { return m_property.entity; }
	const Entity::Property &property() const { return m_property; }

	void setEntity(Entity *value) { m_property.entity = value; }
	void setName(const QString &name) { m_property.name = name; }

private:
	Entity::Property m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITIESLISTMODELITEM_H_ */
