#ifndef ARHPLUGIN_H_
#define ARHPLUGIN_H_

#include "arhplugin_ns.h"
#include "../../filesystem/filesystempluginsmanager.h"


ARH_PLUGIN_NS_BEGIN

class ArhPlugin : public PluginsManager::Plugin
{
public:
	ArhPlugin();

	virtual Node *node(const IFileInfo *info, Node *parent) const;
	virtual Node *node(const IFileInfo *info, IFile *file, Node *parent) const;

private:
	QString m_identity;
};

ARH_PLUGIN_NS_END

#endif /* ARHPLUGIN_H_ */
