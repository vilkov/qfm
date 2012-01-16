#ifndef ARCNODEITEM_H_
#define ARCNODEITEM_H_

#include "../../arcplugin_ns.h"
#include "../../../../filesystem/tasks/items/filesystemtasknodeitem.h"


ARC_PLUGIN_NS_BEGIN

class ArcNodeItem : public TaskNodeItem
{
public:
	ArcNodeItem(Base *parent);

	virtual bool isRoot() const = 0;
	virtual bool isDir() const = 0;

	void updateProgress(quint64 value, quint64 timeElapsed) { progress(value, timeElapsed); }
};


class ArcNodeListItem : public ArcNodeItem
{
public:
	typedef QList<ArcNodeItem *> Container;

public:
	ArcNodeListItem(Base *parent);

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

protected:
	Container m_items;
};

ARC_PLUGIN_NS_END

#endif /* ARCNODEITEM_H_ */
