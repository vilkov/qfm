#ifndef IDM_COMPOSITEVALUEVALUEITEM_H_
#define IDM_COMPOSITEVALUEVALUEITEM_H_

#include "idm_compositevalueitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueValueItem : public CompositeValueItem
{
public:
	CompositeValueValueItem(const IdmEntityValue::Holder &value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValueItem */
	virtual bool isValue() const;

	const IdmEntityValue::Holder &value() const { return m_value; }
	IdmEntityValue::Holder take() { IdmEntityValue::Holder res(m_value); m_value.reset(); return res; }

protected:
	IdmEntityValue::Holder m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEVALUEITEM_H_ */
