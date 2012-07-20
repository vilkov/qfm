#ifndef IDMQUERYRESULTPATHITEM_H_
#define IDMQUERYRESULTPATHITEM_H_

#include <vfs/vfs_node.h>
#include "idmqueryresultitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPathItem : public QueryResultItem
{
public:
	typedef QList<QueryResultItem::Holder> Container;

public:
	QueryResultPathItem(IFileInfo::Holder &info, Base *parent);
	QueryResultPathItem(const IFileContainer *container, const QString &fileName, Base *parent);

	/* Base */
	virtual Base *at(Base::size_type index) const;
	virtual Base::size_type size() const;
	virtual Base::size_type indexOf(Base *item) const;
	virtual QVariant data(qint32 column, qint32 role) const;

	/* QueryResultItem */
	virtual bool isRoot();
	virtual bool isProperty();
	virtual bool isValue();
	virtual bool isPath();

	const IFileInfo::Holder &info() const { return m_info; }
	IFileInfo::Holder &info() { return m_info; }

	virtual void open() const = 0;

	Node *node() const { return m_node; }
	void setNode(Node *node) { m_node = node; }

protected:
	IFileInfo::Holder m_info;
	Node *m_node;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPATHITEM_H_ */
