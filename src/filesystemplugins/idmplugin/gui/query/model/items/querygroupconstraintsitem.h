#ifndef QUERYGROUPCONSTRAINTSITEM_H_
#define QUERYGROUPCONSTRAINTSITEM_H_

#include "../../../../model/items/idmlistitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryGroupConstraintsItem : public IdmListItem
{
public:
	QueryGroupConstraintsItem(IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

private:
	QVariant m_value;
};

IDM_PLUGIN_NS_END

#endif /* QUERYGROUPCONSTRAINTSITEM_H_ */
