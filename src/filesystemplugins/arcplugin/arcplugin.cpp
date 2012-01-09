#include "arcplugin.h"
#include "nodes/arcnode.h"


ARC_PLUGIN_NS_BEGIN

ArcPlugin::ArcPlugin()
{}

Node *ArcPlugin::node(const IFileInfo *info, Node *parent) const
{
	return new ArcNode(Info(info->absoluteFilePath(), true), parent);
}

ARC_PLUGIN_NS_END
