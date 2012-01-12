#ifndef ARCPLUGIN_H_
#define ARCPLUGIN_H_

#include "plugins/arclibarchiveplugin.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


ARC_PLUGIN_NS_BEGIN

class Plugin : public IFileReaderPlugin
{
public:
	Plugin();

	virtual Node *node(const IFileInfo *info, Node *parent) const;
	virtual FileTypeIdList fileTypes() const;

	static const Archive **archivers();

private:
	LibArchivePlugin m_libArchive;
	const Archive *m_archivers[2];
};

ARC_PLUGIN_NS_END

#endif /* ARCPLUGIN_H_ */
