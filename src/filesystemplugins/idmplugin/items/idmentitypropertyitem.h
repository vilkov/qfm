#ifndef IDMENTITYPROPERTYITEM_H_
#define IDMENTITYPROPERTYITEM_H_

#include "idmentityitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityPropertyItem : public IdmEntityItem
{
public:
	IdmEntityPropertyItem(const IdmEntity::Property &property, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	const IdmEntity::Property &property() const { return m_property; }

protected:
	IdmEntity::Property m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITYPROPERTYITEM_H_ */
