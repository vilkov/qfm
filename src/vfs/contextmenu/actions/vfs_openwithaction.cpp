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
#include "vfs_openwithaction.h"
#include <application.h>


VFS_NS_BEGIN

OpenWithAction::OpenWithAction(const IApplication *application) :
	SyncFileAction(application->icon(), label(application)),
	m_application(application)
{}

void OpenWithAction::process(const IFileContainer *container, const FilesList &files) const
{
	Application::open(m_application, container, files.at(0).second);
}

QString OpenWithAction::label(const IApplication *application)
{
	QString description = application->genericName().isEmpty() ? application->description() : application->genericName();

	if (description.isEmpty())
		return application->name();
	else
		return QString::fromLatin1("%1 (%2)").arg(application->name()).arg(description);
}

VFS_NS_END
