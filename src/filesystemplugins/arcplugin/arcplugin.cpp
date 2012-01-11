#include "arcplugin.h"
#include "nodes/arcnode.h"
#include "../../application.h"


ARC_PLUGIN_NS_BEGIN

ArcPlugin::ArcPlugin()
{}

Node *ArcPlugin::node(const IFileInfo *info, Node *parent) const
{
	return new ArcNode(Info(info->absoluteFilePath(), true), parent);
}

ArcPlugin::FileTypeIdList ArcPlugin::fileTypes() const
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

ARC_PLUGIN_NS_END
