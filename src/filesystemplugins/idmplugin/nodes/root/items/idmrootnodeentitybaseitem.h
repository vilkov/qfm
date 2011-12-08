#ifndef IDMROOTNODEENTITYBASEITEM_H_
#define IDMROOTNODEENTITYBASEITEM_H_

#include "idmrootnodeitem.h"
#include "../../../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeEntityBaseItem : public RootNodeItem
{
public:
	RootNodeEntityBaseItem(Base *parent);

	/* IRootNodeItem */
	virtual bool isRoot();
	virtual bool isFiles();
};


class RootNodeEntityBaseListItem : public RootNodeListItem
{
public:
	RootNodeEntityBaseListItem(Base *parent);

	/* IRootNodeItem */
	virtual bool isRoot();
	virtual bool isFiles();
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODEENTITYBASEITEM_H_ */
