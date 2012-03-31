#ifndef COMPOSITEVALUEPATHITEM_H_
#define COMPOSITEVALUEPATHITEM_H_

#include "compositevalueitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePathItem : public CompositeValueItem
{
public:
	CompositeValuePathItem(const IdmEntityValue::Holder &value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValueItem */
	virtual bool isPath() const;

	const IdmEntityValue::Holder &value() const { return m_value; }
	IdmEntityValue::Holder take() { IdmEntityValue::Holder res(m_value); m_value.reset(); return res; }

protected:
	IdmEntityValue::Holder m_value;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEPATHITEM_H_ */
