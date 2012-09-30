#ifndef IDM_QUERYRESULTROOTITEM_H_
#define IDM_QUERYRESULTROOTITEM_H_

#include "idm_queryresultitem.h"
#include "../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultRootItem : public QueryResultItem
{
public:
	QueryResultRootItem(const EntityValue::Holder &value, Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRoot();
	virtual bool isProperty();
	virtual bool isValue();
	virtual bool isPath();
	virtual bool isRootPathValue();

	const EntityValue::Holder &value() const { return m_value; }

private:
	EntityValue::Holder m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTROOTITEM_H_ */
