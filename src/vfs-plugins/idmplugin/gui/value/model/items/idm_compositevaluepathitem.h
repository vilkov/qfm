#ifndef IDM_COMPOSITEVALUEPATHITEM_H_
#define IDM_COMPOSITEVALUEPATHITEM_H_

#include "idm_compositevalueitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePathItem : public CompositeValueItem
{
public:
	CompositeValuePathItem(const IdmEntityValue::Holder &value, IdmItem *parent = 0);

	/* CompositeValueItem */
	virtual bool isPath() const;

	virtual QString fileName() const = 0;
	virtual bool isFile() const = 0;
	virtual void open() const = 0;

	const IdmEntityValue::Holder &value() const { return m_value; }
	IdmEntityValue::Holder take() { IdmEntityValue::Holder res(m_value); m_value.reset(); return res; }

protected:
	IdmEntityValue::Holder m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEPATHITEM_H_ */
