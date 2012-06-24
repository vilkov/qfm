#ifndef ARCPLUGIN_H_
#define ARCPLUGIN_H_

#include "libarchive/actions/libarchiveunpackintosubdiraction.h"
#include "libunrar/actions/libunrarunpackintosubdiraction.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"
#include "../../tools/settings/options/settingsoption.h"


ARC_PLUGIN_NS_BEGIN

class ArcPlugin : public IFilePlugin
{
public:
	ArcPlugin(::Tools::Settings::Option *parentOption);

	/* IPlugin */
	virtual ::Tools::Settings::Page *settings();
};


class LibArchivePlugin : public ArcPlugin
{
public:
	LibArchivePlugin(::Tools::Settings::Option *parentOption);

	/* IPlugin */
	virtual void registered();

	/* IFilePlugin */
	virtual FileTypeIdList fileTypes() const;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

private:
	LibArchive::UnPackIntoSubdirAction m_unPackIntoSubdirAction;
};


class LibUnRarPlugin : public ArcPlugin
{
public:
	LibUnRarPlugin(::Tools::Settings::Option *parentOption);

	/* IPlugin */
	virtual void registered();

	/* IFilePlugin */
	virtual FileTypeIdList fileTypes() const;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

private:
	LibUnrar::UnPackIntoSubdirAction m_unPackIntoSubdirAction;
};

ARC_PLUGIN_NS_END

#endif /* ARCPLUGIN_H_ */
