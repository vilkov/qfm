#ifndef IDMQUERYRESULTPATHPROPERTYITEM_H_
#define IDMQUERYRESULTPATHPROPERTYITEM_H_

#include <vfs/tasks/vfs_tasksnode.h>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "idmqueryresultpropertyitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPathPropertyItem : public QueryResultPropertyItem
{
public:
	QueryResultPathPropertyItem(const IdmEntity::Property &property, Base *parent);

	void add(const IFileContainer *container, const IdmEntityValue::Holder &value);
	void add(const IFileContainer *container, const IdmCompositeEntityValue::List &values);
	void add(Snapshot::Files &files, const IFileContainer *container, const IdmCompositeEntityValue::List &values);
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPATHPROPERTYITEM_H_ */
