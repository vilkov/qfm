#ifndef COMPOSITEVALUEVALUEITEM_H_
#define COMPOSITEVALUEVALUEITEM_H_

#include "compositevalueitem.h"
#include "../../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueValueItem : public CompositeValueItem
{
public:
	CompositeValueValueItem(IdmEntityValue *value, IdmItem *parent = 0);
	virtual ~CompositeValueValueItem();

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

protected:
	friend class CompositeValueModel;
	void add(CompositeValueItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }

private:
	IdmEntityValue *m_value;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEVALUEITEM_H_ */
