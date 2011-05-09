#ifndef M3UPLUGIN_H_
#define M3UPLUGIN_H_

#include "../../filesystem/filesystempluginsmanager.h"


class M3uPlugin : public FileSystem::PluginsManager::FilePlugin
{
public:
	M3uPlugin();

	virtual FileSystem::Node *node(const FileSystem::Info &info, File file, size_type size, FileSystem::Node *parent) const;

private:
	QString m_identity;
};

#endif /* M3UPLUGIN_H_ */
