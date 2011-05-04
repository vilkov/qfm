#ifndef FILESYSTEMMANAGER_DEF_H_
#define FILESYSTEMMANAGER_DEF_H_

#include "plugins/filesystemfolderplugin.h"
#include "../filesystem/manager/filesystemmanager.h"


class FileSystemManagerDef : public FileSystemManager
{
public:
	FileSystemManagerDef();

private:
	FileSystemFolderPlugin m_folderPlugin;
};

#endif /* FILESYSTEMMANAGER_DEF_H_ */
