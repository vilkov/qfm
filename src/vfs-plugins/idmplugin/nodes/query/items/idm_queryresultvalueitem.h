#ifndef IDM_QUERYRESULTVALUEITEM_H_
#define IDM_QUERYRESULTVALUEITEM_H_

#include <QtGui/QIcon>
#include "idm_queryresultitem.h"
#include "../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultValueItem : public QueryResultItem
{
public:
	QueryResultValueItem(const EntityValue::Holder &value, Base *parent);

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

#endif /* IDM_QUERYRESULTVALUEITEM_H_ */
