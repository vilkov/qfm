#ifndef ARHPLUGIN_H_
#define ARHPLUGIN_H_

#include "arhplugin_ns.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


ARH_PLUGIN_NS_BEGIN

class ArhPlugin : public IPlugin
{
public:
	ArhPlugin();

	virtual Node *node(const IFileInfo *info, Node *parent) const;
};

ARH_PLUGIN_NS_END

#endif /* ARHPLUGIN_H_ */
