#ifndef IDMPLUGIN_H_
#define IDMPLUGIN_H_

#include "idmplugin_ns.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


IDM_PLUGIN_NS_BEGIN

class IdmPlugin : public IPlugin
{
public:
	IdmPlugin();

	virtual Node *node(const IFileInfo *info, Node *parent) const;
};

IDM_PLUGIN_NS_END

#endif /* IDMPLUGIN_H_ */
