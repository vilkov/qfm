#ifndef IDMQUERYRESULTPATHPROPERTYITEM_H_
#define IDMQUERYRESULTPATHPROPERTYITEM_H_

#include "idmqueryresultpropertyitem.h"
#include "../../../../../filesystem/tasks/filesystemtasksnode.h"
#include "../../../../../filesystem/interfaces/filesystemifilecontainer.h"


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
