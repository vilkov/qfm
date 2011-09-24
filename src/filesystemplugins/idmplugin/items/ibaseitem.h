#ifndef IDMIBASEITEM_H_
#define IDMIBASEITEM_H_

#include "../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class IBaseItem
{
public:
	virtual ~IBaseItem();

	virtual bool isRoot() const = 0;
	virtual bool isMenuItem() const = 0;
	virtual bool isValueItem() const = 0;
	virtual bool isEntityItem() const = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDMIBASEITEM_H_ */
