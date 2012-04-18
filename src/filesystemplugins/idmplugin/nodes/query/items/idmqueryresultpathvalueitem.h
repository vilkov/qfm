#ifndef IDMQUERYRESULTPATHVALUEITEM_H_
#define IDMQUERYRESULTPATHVALUEITEM_H_

#include <QtCore/QCoreApplication>
#include "idmqueryresultpathitem.h"
#include "../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPathValueItem : public QueryResultPathItem
{
	Q_DECLARE_TR_FUNCTIONS(QueryResultPathValueItem)

public:
	QueryResultPathValueItem(const IFileContainer *container, const InfoItem *item, Base *parent);

	/* QueryResultPathItem */
	virtual void open() const;

private:
	const IFileContainer *m_container;
	IFileContainer::Holder m_thisContainer;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPATHVALUEITEM_H_ */
