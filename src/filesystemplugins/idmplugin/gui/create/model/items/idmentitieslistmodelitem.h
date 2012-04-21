#ifndef IDMENTITIESLISTMODELITEM_H_
#define IDMENTITIESLISTMODELITEM_H_

#include "../../../../model/items/idmlistitem.h"
#include "../../../../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntitiesListItem : public IdmListItem
{
public:
	IdmEntitiesListItem(const IdmEntity::Property &property, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	IdmEntity *entity() const { return m_property.entity; }
	const IdmEntity::Property &property() const { return m_property; }

	void setEntity(IdmEntity *value) { m_property.entity = value; }
	void setName(const QString &name) { m_property.name = name; }

private:
	IdmEntity::Property m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITIESLISTMODELITEM_H_ */
