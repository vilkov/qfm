/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "arc_plugin.h"
#include "nodes/arc_node.h"
#include "settings/arc_pluginsettings.h"
#include "libarchive/interfaces/arc_libarchive_filecontainer.h"
#include "libunrar/interfaces/arc_libunrar_filecontainer.h"
#include "../../application.h"


ARC_PLUGIN_NS_BEGIN
static Settings *globalSettings;


Plugin::Plugin(::Tools::Settings::Option *parentOption)
{
	static Settings settings(parentOption);
	globalSettings = &settings;
}

::Tools::Settings::Page *Plugin::settings()
{
	return globalSettings;
}

ARC_PLUGIN_NS_END


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

Plugin::Plugin(::Tools::Settings::Option *parentOption) :
	Arc::Plugin(parentOption)
{}

void Plugin::registered()
{
	FileTypeIdList list = fileTypes();
	Application::globalMenu()->registerAction(&m_unPackIntoSubdirAction, list);
}

Plugin::FileTypeIdList Plugin::fileTypes() const
{
	FileTypeIdList res;
	FileTypeId type;

	type.mime = QByteArray("application/x-gzip");
	res.push_back(type);

	type.mime = QByteArray("application/x-tar");
	res.push_back(type);

	type.mime = QByteArray("application/x-compressed-tar");
	res.push_back(type);

	type.mime = QByteArray("application/x-bzip-compressed-tar");
	res.push_back(type);

	type.mime = QByteArray("application/zip");
	res.push_back(type);

	type.mime = QByteArray("application/x-bzip");
	res.push_back(type);

	type.mime = QByteArray("application/x-tarz");
	res.push_back(type);

	type.mime = QByteArray("application/x-bzip2");
	res.push_back(type);

	type.mime = QByteArray("application/x-java-archive");
	res.push_back(type);

	type.mime = QByteArray("application/x-deb");
	res.push_back(type);

	type.mime = QByteArray("application/x-rpm");
	res.push_back(type);

	type.mime = QByteArray("application/x-7z-compressed");
	res.push_back(type);

	type.mime = QByteArray("application/x-compress");
	res.push_back(type);

	type.mime = QByteArray("application/x-zip-compressed");
	res.push_back(type);

	type.mime = QByteArray("application/x-lzma");
	res.push_back(type);

	type.mime = QByteArray("application/x-servicepack");
	res.push_back(type);

	type.mime = QByteArray("application/x-xz-compressed-tar");
	res.push_back(type);

	type.mime = QByteArray("application/x-lzma-compressed-tar");
	res.push_back(type);

	type.mime = QByteArray("application/x-cd-image");
	res.push_back(type);

	return res;
}

Node *Plugin::open(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	QString error;
	IFileContainer::Holder localContainer(FileContainer::create(container, file, error));

	if (localContainer)
		return new ArcNode(localContainer, parent);
	else
		return NULL;
}

ARC_PLUGIN_LIBARCHIVE_NS_END


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

Plugin::Plugin(::Tools::Settings::Option *parentOption) :
	Arc::Plugin(parentOption)
{}

void Plugin::registered()
{
	FileTypeIdList list = fileTypes();
	Application::globalMenu()->registerAction(&m_unPackIntoSubdirAction, list);
}

Plugin::FileTypeIdList Plugin::fileTypes() const
{
	FileTypeIdList res;
	FileTypeId type;

	type.mime = QByteArray("application/x-rar");
	res.push_back(type);

	return res;
}

Node *Plugin::open(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	QString error;
	IFileContainer::Holder localContainer(FileContainer::create(container, file, error));

	if (localContainer)
		return new ArcNode(localContainer, parent);
	else
		return NULL;
}

ARC_PLUGIN_LIBUNRAR_NS_END
