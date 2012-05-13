#ifndef ARCPLUGIN_H_
#define ARCPLUGIN_H_

#include "actions/arcpackaction.h"
#include "actions/arcunpackaction.h"
#include "actions/arcunpackhereaction.h"
#include "actions/arcunpackintosubdiraction.h"
#include "plugins/arclibarchiveplugin.h"
#include "settings/arcpluginsettings.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


ARC_PLUGIN_NS_BEGIN

class Plugin : public IFilePlugin
{
public:
	Plugin();

	/* IPlugin */
	virtual void registered();
	virtual const ::Tools::Settings::Tab *settings() const;

	/* IFilePlugin */
	virtual FileTypeIdList fileTypes() const;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

	static const Archive **archivers();

private:
	Settings m_settings;
	PackAction m_packAction;
	UnPackAction m_unPackAction;
	UnPackHereAction m_unPackHereAction;
	UnPackIntoSubdirAction m_unPackIntoSubdirAction;
	LibArchivePlugin m_libArchive;
	const Archive *m_archivers[2];
};

ARC_PLUGIN_NS_END

#endif /* ARCPLUGIN_H_ */
