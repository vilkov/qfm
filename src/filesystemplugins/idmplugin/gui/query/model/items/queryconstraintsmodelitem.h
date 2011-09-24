#ifndef QUERYCONSTRAINTSMODELITEM_H_
#define QUERYCONSTRAINTSMODELITEM_H_

#include <QtCore/QVariant>
#include "../../../../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class QueryConstraintsModelItem
{
public:
	QueryConstraintsModelItem(QueryConstraintsModelItem *parent = 0);
	virtual ~QueryConstraintsModelItem();

	QueryConstraintsModelItem *parent() const { return m_parent; }

	virtual bool isGroup() const = 0;
	virtual QVariant data(qint32 column, qint32 role) const = 0;

private:
	QueryConstraintsModelItem *m_parent;
};

IDM_PLUGIN_NS_END

#endif /* QUERYCONSTRAINTSMODELITEM_H_ */
