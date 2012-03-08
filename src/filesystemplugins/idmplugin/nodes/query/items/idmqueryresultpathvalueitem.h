#ifndef IDMQUERYRESULTPATHVALUEITEM_H_
#define IDMQUERYRESULTPATHVALUEITEM_H_

#include "idmqueryresultvalueitem.h"
#include "../../../../../filesystem/filesystemnode.h"
#include "../../../../../filesystem/info/filesysteminfo.h"
#include "../../../../../filesystem/interfaces/filesystemifilecontainer.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPathValueItem : public QueryResultValueItem
{
public:
	QueryResultPathValueItem(const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	const Info &info() const { return m_info; }
	Info &info() { return m_info; }

	void update() { m_info = value()->value().toString(); }

	Node *node() const { return m_node; }
	void setNode(Node *node) { m_node = node; }

private:
	Info m_info;
	Node *m_node;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPATHVALUEITEM_H_ */
