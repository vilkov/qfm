#ifndef IDMENTITYITEM_H_
#define IDMENTITYITEM_H_

#include "idmitemslist.h"
#include "../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityItem : public IdmItemsList
{
public:
	IdmEntityItem(IdmEntity *entity, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isEntityItem() const;

	IdmEntity *entity() const { return m_entity; }

protected:
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITYITEM_H_ */
