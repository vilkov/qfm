#ifndef IDMPLUGIN_H_
#define IDMPLUGIN_H_

#include "idmplugin_ns.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


IDM_PLUGIN_NS_BEGIN

class Plugin : public IPlugin
{
public:
	Plugin();

	virtual Node *node(const IFileInfo *info, Node *parent) const;
};

IDM_PLUGIN_NS_END

#endif /* IDMPLUGIN_H_ */
