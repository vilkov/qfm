#include "idmplugin.h"
#include "nodes/idmrootnode.h"


FILE_SYSTEM_NS_BEGIN

IdmPlugin::IdmPlugin()
{

}

Node *IdmPlugin::node(const IFileInfo *info, Node *parent) const
{
	if (info->absoluteFilePath() == QString::fromLatin1("/home/dav/idm"))
		return new IdmRootNode(info->absoluteFilePath(QString::fromLatin1(".storage.idm")), parent);
	else
		return 0;
}

Node *IdmPlugin::node(const IFileInfo *info, IFile *file, Node *parent) const
{
	return 0;
}

FILE_SYSTEM_NS_END
