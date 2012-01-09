#ifndef ARCPLUGIN_H_
#define ARCPLUGIN_H_

#include "arcplugin_ns.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


ARC_PLUGIN_NS_BEGIN

class ArcPlugin : public IFileReaderPlugin
{
public:
	ArcPlugin();

	virtual Node *node(const IFileInfo *info, Node *parent) const;
	virtual FileTypeIdList fileTypes() const;
};

ARC_PLUGIN_NS_END

#endif /* ARCPLUGIN_H_ */
