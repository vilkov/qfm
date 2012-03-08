#ifndef IDMQUERYRESULTROOTITEM_H_
#define IDMQUERYRESULTROOTITEM_H_

#include "idmqueryresultitem.h"
#include "../../../storage/values/idmentityvalue.h"
#include "../../../../../filesystem/tasks/filesystemtasksnode.h"
#include "../../../../../filesystem/interfaces/filesystemifilecontainer.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultRootItem : public QueryResultListItem
{
public:
	QueryResultRootItem(TasksNode::TasksItemList &files, const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRoot();
	virtual bool isProperty();
	virtual bool isValue();

	const IdmEntityValue::Holder &value() const { return m_value; }

private:
	IdmEntityValue::Holder m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTROOTITEM_H_ */
