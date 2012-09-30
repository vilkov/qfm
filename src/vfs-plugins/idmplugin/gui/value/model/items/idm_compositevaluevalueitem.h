#ifndef IDM_COMPOSITEVALUEVALUEITEM_H_
#define IDM_COMPOSITEVALUEVALUEITEM_H_

#include "idm_compositevalueitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueValueItem : public CompositeValueItem
{
public:
	CompositeValueValueItem(const EntityValue::Holder &value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValueItem */
	virtual bool isValue() const;

	const EntityValue::Holder &value() const { return m_value; }
	EntityValue::Holder take() { EntityValue::Holder res(m_value); m_value.reset(); return res; }

protected:
	EntityValue::Holder m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEVALUEITEM_H_ */
