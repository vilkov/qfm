#include "idmplugin.h"
#include "nodes/root/idmrootnode.h"


IDM_PLUGIN_NS_BEGIN

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

IDM_PLUGIN_NS_END
