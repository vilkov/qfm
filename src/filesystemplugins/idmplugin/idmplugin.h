#ifndef IDMPLUGIN_H_
#define IDMPLUGIN_H_

#include "idmplugin_ns.h"
#include "../../filesystem/filesystempluginsmanager.h"


IDM_PLUGIN_NS_BEGIN

class IdmPlugin : public PluginsManager::Plugin
{
public:
	IdmPlugin();

	virtual Node *node(const IFileInfo *info, Node *parent) const;
	virtual Node *node(const IFileInfo *info, IFile *file, Node *parent) const;

private:
	QString m_identity;
};

IDM_PLUGIN_NS_END

#endif /* IDMPLUGIN_H_ */
