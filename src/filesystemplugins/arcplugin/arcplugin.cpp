#include "arcplugin.h"
#include "nodes/arcnode.h"
#include "../../application.h"


ARC_PLUGIN_NS_BEGIN
static Plugin *instance = 0;


Plugin::Plugin()
{
	m_archivers[0] = &m_libArchive;
	m_archivers[1] = 0;
	instance = this;
}

void Plugin::registered()
{
	Plugin::FileTypeIdList list = fileTypes();

	Application::globalMenu()->registerAction(&m_unPackAction, list);
	Application::globalMenu()->registerAction(&m_unPackHereAction, list);
	Application::globalMenu()->registerAction(&m_unPackIntoSubdirAction, list);

	Application::globalMenu()->registerAction(&m_packAction, ::DesktopEnvironment::ContextMenuFactory::AnyFilesOrFolders);
}

const ::Tools::Settings::Tab *Plugin::settings() const
{
	return &m_settings;
}

Plugin::FileTypeIdList Plugin::fileTypes() const
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

	type.mime = QString::fromLatin1("application/x-rar");
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

Node *Plugin::open(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	return new ArcNode(container->location(file->fileName()), parent);
}

const Archive **Plugin::archivers()
{
	return instance->m_archivers;
}

ARC_PLUGIN_NS_END
