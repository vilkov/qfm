#include "filesystemmanager_def.h"


FileSystemManagerDef::FileSystemManagerDef() :
	FileSystemManager()
{
	registerStaticPlugin(&m_folderPlugin);
}
