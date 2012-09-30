#ifndef IDM_QUERYRESULTPATHPROPERTYITEM_H_
#define IDM_QUERYRESULTPATHPROPERTYITEM_H_

#include <vfs/tasks/vfs_tasksnode.h>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "idm_queryresultpropertyitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPathPropertyItem : public QueryResultPropertyItem
{
public:
	QueryResultPathPropertyItem(const Entity::Property &property, Base *parent);

	void add(const IFileContainer *container, const EntityValue::Holder &value);
	void add(const IFileContainer *container, const CompositeEntityValue::List &values);
	void add(Snapshot::Files &files, const IFileContainer *container, const CompositeEntityValue::List &values);
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTPATHPROPERTYITEM_H_ */
