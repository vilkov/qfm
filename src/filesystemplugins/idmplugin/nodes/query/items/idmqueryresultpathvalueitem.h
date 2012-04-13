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
	QueryResultPathValueItem(const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent);

	/* IFileInfo */
	virtual QString fileName() const;

	void update(const InfoItem *item);

	Node *node() const { return m_node; }
	void setNode(Node *node) { m_node = node; }

private:
	Node *m_node;
	IdmEntityValue::Holder m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPATHVALUEITEM_H_ */
