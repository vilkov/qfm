#ifndef IDMQUERYRESULTPATHITEM_H_
#define IDMQUERYRESULTPATHITEM_H_

#include "idmqueryresultitem.h"
#include "../../../../../filesystem/filesystemnode.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPathItem : public QueryResultItem, public IFileInfo
{
public:
	typedef QList<QueryResultPathItem *> Container;

public:
	QueryResultPathItem(IFileInfo::Holder &info, Base *parent);
	QueryResultPathItem(const IFileContainer *container, const QString &fileName, Base *parent);
	virtual ~QueryResultPathItem();

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

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual IFileInfo::size_type fileSize() const;
	virtual QString fileName() const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;

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
