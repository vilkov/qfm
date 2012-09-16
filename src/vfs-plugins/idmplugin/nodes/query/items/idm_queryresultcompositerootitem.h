#ifndef IDM_QUERYRESULTCOMPOSITEROOTITEM_H_
#define IDM_QUERYRESULTCOMPOSITEROOTITEM_H_

#include <QtCore/QVector>
#include <vfs/tasks/vfs_tasksnode.h>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "idm_queryresultrootitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultCompositeRootItem : public QueryResultRootItem
{
public:
	typedef QVector<QueryResultItem::Holder> Container;

public:
	QueryResultCompositeRootItem(Snapshot::Files &files, const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent = 0);

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

private:
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTCOMPOSITEROOTITEM_H_ */
