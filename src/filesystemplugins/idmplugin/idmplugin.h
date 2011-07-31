#ifndef IDMPLUGIN_H_
#define IDMPLUGIN_H_

#include "../../filesystem/filesystempluginsmanager.h"


FILE_SYSTEM_NS_BEGIN

class IdmPlugin : public PluginsManager::Plugin
{
public:
	IdmPlugin();

	virtual Node *node(const IFileInfo *info, Node *parent) const;
	virtual Node *node(const IFileInfo *info, IFile *file, Node *parent) const;

private:
	QString m_identity;
};

FILE_SYSTEM_NS_END

#endif /* IDMPLUGIN_H_ */
