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

Node *Plugin::node(const IFileInfo *info, Node *parent) const
{
	return new ArcNode(Info(info->absoluteFilePath(), true), parent);
}

Plugin::FileTypeIdList Plugin::fileTypes() const
{
	const DesktopEnvironment &de = *Application::desktopEnvironment();

	return FileTypeIdList() <<
			de.fileTypeId(DesktopEnvironment::FileTypes::Application::GZipFile) <<
			de.fileTypeId(DesktopEnvironment::FileTypes::Application::TarFile) <<
			de.fileTypeId(DesktopEnvironment::FileTypes::Application::CompressedTarFile) <<
			de.fileTypeId(DesktopEnvironment::FileTypes::Application::BZipCompressedTarFile) <<
			de.fileTypeId(DesktopEnvironment::FileTypes::Application::ZipFile) <<
			de.fileTypeId(DesktopEnvironment::FileTypes::Application::BZipFile) <<
			de.fileTypeId(DesktopEnvironment::FileTypes::Application::RarFile) <<
			de.fileTypeId(DesktopEnvironment::FileTypes::Application::TarzFile) <<
			de.fileTypeId(DesktopEnvironment::FileTypes::Application::BZip2File);
}

const Archive **Plugin::archivers()
{
	return instance->m_archivers;
}

ARC_PLUGIN_NS_END
