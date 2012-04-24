#ifndef IDMROOTNODEITEM_H_
#define IDMROOTNODEITEM_H_

#include "../../../idmplugin_ns.h"
#include "../../../../../filesystem/tasks/items/filesystemtasknodeitem.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeItem : public TasksNodeItem
{
public:
	RootNodeItem(Base *parent);

	virtual bool isRoot();
	virtual bool isFiles();
	virtual bool isEntity();
	virtual bool isProperty();
};


class RootNodeListItem : public RootNodeItem
{
public:
	typedef QList<RootNodeItem *> Container;

public:
	RootNodeListItem(Base *parent);

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

protected:
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODEITEM_H_ */
