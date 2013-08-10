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
#include "default_pluginsettings.h"


DEFAULT_PLUGIN_NS_BEGIN
Settings *s_instance;


Settings::Settings(Option *parent) :
	Page(tr("Default"), QString::fromLatin1("Default"), parent),
	m_text(tr("This plugin is the default embedded plugin which can handle regular folders and files.")),
	m_fileIcon(this)
{
	Q_ASSERT(s_instance == NULL);

	manage(&m_text);
	manage(&m_fileIcon, &m_fileIcon);

	s_instance = this;
}

Settings::FileIcon::FileIcon(Option *parent) :
	GroupBox(tr("File icon"), QString::fromLatin1("FileIcon"), parent),
	m_onlyAppIcon(tr("Use only icon of application"), QString::fromLatin1("OnlyAppIcon"), this, false),
	m_onlyTypeIcon(tr("Use only icon of type"), QString::fromLatin1("OnlyTypeIcon"), this, false),
	m_appIconIfNoTypeIcon(tr("Use icon of application only if there is no icon of type"), QString::fromLatin1("AppIconIfNoTypeIcon"), this, true)
{
	manage(&m_onlyAppIcon, &m_onlyAppIcon);
	manage(&m_onlyTypeIcon, &m_onlyTypeIcon);
	manage(&m_appIconIfNoTypeIcon, &m_appIconIfNoTypeIcon);
}

Settings *Settings::instance()
{
	Q_ASSERT(s_instance);
	return s_instance;
}

DEFAULT_PLUGIN_NS_END
