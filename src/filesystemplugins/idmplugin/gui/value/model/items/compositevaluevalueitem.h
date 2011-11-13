#ifndef COMPOSITEVALUEVALUEITEM_H_
#define COMPOSITEVALUEVALUEITEM_H_

#include "../../../../model/items/idmitem.h"
#include "../../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueValueItem : public IdmItem
{
public:
	CompositeValueValueItem(IdmEntityValue *value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	IdmEntityValue *value() const { return m_value; }
	IdmEntityValue *take() { IdmEntityValue *res = m_value; m_value = 0; return res; }

protected:
	IdmEntityValue *m_value;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEVALUEITEM_H_ */
