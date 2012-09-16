#ifndef IDM_QUERYRESULTPATHVALUEITEM_H_
#define IDM_QUERYRESULTPATHVALUEITEM_H_

#include <QtCore/QCoreApplication>
#include "idm_queryresultpathitem.h"
#include "../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPathValueItem : public QueryResultPathItem
{
	Q_DECLARE_TR_FUNCTIONS(QueryResultPathValueItem)

public:
	QueryResultPathValueItem(const IFileContainer *container, SnapshotItem *item, Base *parent);

	/* QueryResultItem */
	virtual bool isRootPathValue();

	/* QueryResultPathItem */
	virtual void open() const;

private:
	const IFileContainer *m_container;
	IFileContainer::Holder m_thisContainer;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTPATHVALUEITEM_H_ */
