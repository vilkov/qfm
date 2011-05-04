#ifndef FILESYSTEMFOLDERPLUGIN_H_
#define FILESYSTEMFOLDERPLUGIN_H_

#include "../../filesystem/manager/filesystemmanager.h"


class FileSystemFolderPlugin : public FileSystemManager::Plugin
{
public:
	FileSystemFolderPlugin()
	{}

	virtual FileSystemTree *parse(const QString &absoluteFilePath);
};

#endif /* FILESYSTEMFOLDERPLUGIN_H_ */
