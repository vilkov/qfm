#ifndef IDM_QUERYRESULTROOTPATHVALUEITEM_H_
#define IDM_QUERYRESULTROOTPATHVALUEITEM_H_

#include <QtCore/QCoreApplication>
#include "idm_queryresultpathitem.h"
#include "../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultRootPathValueItem : public QueryResultPathItem
{
	Q_DECLARE_TR_FUNCTIONS(QueryResultPathValueItem)

public:
	QueryResultRootPathValueItem(const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent);

	/* QueryResultItem */
	virtual bool isRootPathValue();

	/* QueryResultPathItem */
	virtual void open() const;

	void update(SnapshotItem *item);
	const IdmEntityValue::Holder &value() const { return m_value; }

private:
	IdmEntityValue::Holder m_value;
	const IFileContainer *m_container;
	IFileContainer::Holder m_thisContainer;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTROOTPATHVALUEITEM_H_ */
