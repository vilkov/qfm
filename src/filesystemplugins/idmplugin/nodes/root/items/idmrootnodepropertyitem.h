#ifndef IDMROOTNODEPROPERTYITEM_H_
#define IDMROOTNODEPROPERTYITEM_H_

#include "idmrootnodeentityitem.h"


IDM_PLUGIN_NS_BEGIN

class RootNodePropertyItem : public RootNodeEntityItem
{
public:
	RootNodePropertyItem(const IdmEntity::Property &property, Base *parent);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* IRootNodeItem */
	virtual bool isProperty();

private:
	const IdmEntity::Property &m_property;
	QVariant m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODEPROPERTYITEM_H_ */
