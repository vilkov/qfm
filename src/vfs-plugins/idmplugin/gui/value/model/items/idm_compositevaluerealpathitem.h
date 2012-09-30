#ifndef IDM_COMPOSITEVALUEREALPATHITEM_H_
#define IDM_COMPOSITEVALUEREALPATHITEM_H_

#include "idm_compositevaluepathitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueRealPathItem : public CompositeValuePathItem
{
public:
	CompositeValueRealPathItem(const EntityValue::Holder &value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValuePathItem */
	virtual QString fileName() const;
	virtual bool isFile() const;
	virtual void open() const;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEREALPATHITEM_H_ */
