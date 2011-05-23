#include "filesystemrootnode.h"
#include <QtCore/QStringList>


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode(PluginsManager *plugins) :
#ifdef Q_OS_WIN
	FolderNode(Info()),
#else
	FolderNode(QString::fromLatin1("/")),
#endif
	m_plugins(plugins)
{
	Q_ASSERT(m_plugins != 0);
}

FILE_SYSTEM_NS_END
