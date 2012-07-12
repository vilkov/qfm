#ifndef M3UITEM_H_
#define M3UITEM_H_

#include <vfs/interfaces/vfs_ifileinfo.h>
#include "../m3uplugin_ns.h"
#include "../../../tools/models/tree/items/treemodelitem.h"


M3U_PLUGIN_NS_BEGIN

class M3uItem : public ::Tools::Models::Tree::Item, public IFileInfo
{
public:
	M3uItem(M3uItem *parent = 0);

	virtual bool isList() const;
	virtual bool isRoot() const = 0;
};

M3U_PLUGIN_NS_END

#endif /* M3UITEM_H_ */
