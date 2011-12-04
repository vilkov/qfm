#ifndef IDMQUERYRESULTVALUEITEM_H_
#define IDMQUERYRESULTVALUEITEM_H_

#include <QtGui/QIcon>
#include "idmqueryresultitem.h"
#include "../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultValueItem : public QueryResultItem
{
public:
	QueryResultValueItem(IdmEntityValue *value, Base *parent);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRoot();
	virtual bool isProperty();
	virtual bool isValue();

	IdmEntityValue *value() const { return m_value; }

private:
	IdmEntityValue *m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTVALUEITEM_H_ */
