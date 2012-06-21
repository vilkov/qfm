#include "arcplugin.h"
#include "nodes/arcnode.h"
#include "libarchive/interfaces/libarchivefilecontainer.h"
#include "libunrar/interfaces/libunrarfilecontainer.h"
#include "../../application.h"


ARC_PLUGIN_NS_BEGIN
static LibArchivePlugin *instance = 0;


LibArchivePlugin::LibArchivePlugin()
{
	instance = this;
}

void LibArchivePlugin::registered()
{
	FileTypeIdList list = fileTypes();
	Application::globalMenu()->registerAction(&m_unPackIntoSubdirAction, list);
}

//const Tools::Settings::Tab *LibArchivePlugin::settings() const
//{
//	return &m_settings;
//}

LibArchivePlugin::FileTypeIdList LibArchivePlugin::fileTypes() const
{
	FileTypeIdList res;
	FileTypeId type;

	type.mime = QString::fromLatin1("application/x-gzip");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-tar");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-compressed-tar");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-bzip-compressed-tar");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/zip");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-bzip");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-tarz");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-bzip2");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-java-archive");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-deb");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-7z-compressed");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-compress");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-zip-compressed");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-lzma");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-servicepack");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-xz-compressed-tar");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-lzma-compressed-tar");
	res.push_back(type);

	type.mime = QString::fromLatin1("application/x-cd-image");
	res.push_back(type);

	return res;
}

Node *LibArchivePlugin::open(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	QString error;
	IFileContainer::Holder localContainer(LibArchive::FileContainer::create(container, file, error));

	if (localContainer)
		return new ArcNode(localContainer, parent);
	else
		return NULL;
}

LibUnRarPlugin::LibUnRarPlugin()
{}

void LibUnRarPlugin::registered()
{
	FileTypeIdList list = fileTypes();
	Application::globalMenu()->registerAction(&m_unPackIntoSubdirAction, list);
}

//const Tools::Settings::Tab *LibUnRarPlugin::settings() const
//{
//	return &m_settings;
//}

LibUnRarPlugin::FileTypeIdList LibUnRarPlugin::fileTypes() const
{
	FileTypeIdList res;
	FileTypeId type;

	type.mime = QString::fromLatin1("application/x-rar");
	res.push_back(type);

	return res;
}

Node *LibUnRarPlugin::open(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	QString error;
	IFileContainer::Holder localContainer(LibUnrar::FileContainer::create(container, file, error));

	if (localContainer)
		return new ArcNode(localContainer, parent);
	else
		return NULL;
}

ARC_PLUGIN_NS_END
