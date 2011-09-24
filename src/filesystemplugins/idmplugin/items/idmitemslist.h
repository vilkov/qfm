#ifndef IDMITEMSLIST_H_
#define IDMITEMSLIST_H_

#include <QtCore/QList>
#include "ibaseitem.h"
#include "../model/items/idmlistitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmItemsList : public IdmListItem, public IBaseItem
{
public:
	IdmItemsList(IdmItem *parent = 0);

	/* IBaseItem */
	virtual bool isRoot() const;
	virtual bool isMenuItem() const;
	virtual bool isValueItem() const;
	virtual bool isEntityItem() const;
};

IDM_PLUGIN_NS_END

#endif /* IDMITEMSLIST_H_ */
