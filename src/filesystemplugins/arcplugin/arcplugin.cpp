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
	return FileTypeIdList() <<
			DesktopEnvironment::FileTypes::fileTypeId(DesktopEnvironment::FileTypes::Application::GZipFile) <<
			DesktopEnvironment::FileTypes::fileTypeId(DesktopEnvironment::FileTypes::Application::TarFile) <<
			DesktopEnvironment::FileTypes::fileTypeId(DesktopEnvironment::FileTypes::Application::CompressedTarFile) <<
			DesktopEnvironment::FileTypes::fileTypeId(DesktopEnvironment::FileTypes::Application::BZipCompressedTarFile) <<
			DesktopEnvironment::FileTypes::fileTypeId(DesktopEnvironment::FileTypes::Application::ZipFile) <<
			DesktopEnvironment::FileTypes::fileTypeId(DesktopEnvironment::FileTypes::Application::BZipFile) <<
			DesktopEnvironment::FileTypes::fileTypeId(DesktopEnvironment::FileTypes::Application::RarFile) <<
			DesktopEnvironment::FileTypes::fileTypeId(DesktopEnvironment::FileTypes::Application::TarzFile) <<
			DesktopEnvironment::FileTypes::fileTypeId(DesktopEnvironment::FileTypes::Application::BZip2File);
}

const Archive **Plugin::archivers()
{
	return instance->m_archivers;
}

ARC_PLUGIN_NS_END
