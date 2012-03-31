#ifndef COMPOSITEVALUEREALPATHITEM_H_
#define COMPOSITEVALUEREALPATHITEM_H_

#include "compositevaluepathitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueRealPathItem : public CompositeValuePathItem
{
public:
	CompositeValueRealPathItem(const IdmEntityValue::Holder &value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValuePathItem */
	virtual void open() const;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEREALPATHITEM_H_ */
