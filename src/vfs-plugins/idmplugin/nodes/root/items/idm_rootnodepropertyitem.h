#ifndef IDM_ROOTNODEPROPERTYITEM_H_
#define IDM_ROOTNODEPROPERTYITEM_H_

#include "idm_rootnodeentityitem.h"


IDM_PLUGIN_NS_BEGIN

class RootNodePropertyItem : public RootNodeEntityItem
{
public:
	RootNodePropertyItem(const Entity::Property &property, Base *parent);
	RootNodePropertyItem(Entity *property, const QString &name, Base *parent);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* RootNodeItem */
	virtual bool isProperty();

	const QString &name() const { return m_property.name; }

private:
	Entity::Property m_property;
	QVariant m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ROOTNODEPROPERTYITEM_H_ */
