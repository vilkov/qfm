#include "idmplugin.h"


FILE_SYSTEM_NS_BEGIN

IdmPlugin::IdmPlugin()
{

}

Node *IdmPlugin::node(const IFileInfo *info, Node *parent) const
{
	return 0;
}

Node *IdmPlugin::node(const IFileInfo *info, IFile *file, Node *parent) const
{
	return 0;
}

FILE_SYSTEM_NS_END
