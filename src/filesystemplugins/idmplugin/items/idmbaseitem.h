#ifndef IDMBASEITEM_H_
#define IDMBASEITEM_H_

#include <QtCore/QVariant>
#include "ibaseitem.h"
#include "../model/items/idmitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmBaseItem : public IdmItem, public IBaseItem
{
public:
	IdmBaseItem(IdmItem *parent);
};

IDM_PLUGIN_NS_END

#endif /* IDMBASEITEM_H_ */
