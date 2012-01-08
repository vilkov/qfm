#include "arhplugin.h"


ARH_PLUGIN_NS_BEGIN

ArhPlugin::ArhPlugin()
{}

Node *ArhPlugin::node(const IFileInfo *info, Node *parent) const
{
	return 0;
}

Node *ArhPlugin::node(const IFileInfo *info, IFile *file, Node *parent) const
{
	return 0;
}

ARH_PLUGIN_NS_END
