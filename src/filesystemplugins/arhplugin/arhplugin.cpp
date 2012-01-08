#include "arhplugin.h"
#include "nodes/arhnode.h"


ARH_PLUGIN_NS_BEGIN

ArhPlugin::ArhPlugin()
{}

Node *ArhPlugin::node(const IFileInfo *info, Node *parent) const
{
	return new ArhNode(Info(info->absoluteFilePath(), true), parent);
}

ARH_PLUGIN_NS_END
