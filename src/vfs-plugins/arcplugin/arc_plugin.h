#ifndef ARC_PLUGIN_H_
#define ARC_PLUGIN_H_

#include <vfs/interfaces/vfs_iplugin.h>
#include "libarchive/actions/arc_libarchive_unpackintosubdiraction.h"
#include "libunrar/actions/arc_libunrar_unpackintosubdiraction.h"
#include "../../tools/settings/options/settingsoption.h"


ARC_PLUGIN_NS_BEGIN

class Plugin : public IFilePlugin
{
public:
	Plugin(::Tools::Settings::Option *parentOption);

	/* IPlugin */
	virtual ::Tools::Settings::Page *settings();
};

ARC_PLUGIN_NS_END


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

class Plugin : public Arc::Plugin
{
public:
	Plugin(::Tools::Settings::Option *parentOption);

	/* IPlugin */
	virtual void registered();

	/* IFilePlugin */
	virtual FileTypeIdList fileTypes() const;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

private:
	UnPackIntoSubdirAction m_unPackIntoSubdirAction;
};

ARC_PLUGIN_LIBARCHIVE_NS_END


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

class Plugin : public Arc::Plugin
{
public:
	Plugin(::Tools::Settings::Option *parentOption);

	/* IPlugin */
	virtual void registered();

	/* IFilePlugin */
	virtual FileTypeIdList fileTypes() const;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

private:
	UnPackIntoSubdirAction m_unPackIntoSubdirAction;
};

ARC_PLUGIN_LIBUNRAR_NS_END


#endif /* ARC_PLUGIN_H_ */
