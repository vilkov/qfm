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
#ifndef DEFAULT_PLUGINSETTINGS_H_
#define DEFAULT_PLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../tools/settings/options/gui/page/settingspage.h"
#include "../../../tools/settings/options/gui/decoration/settingsdecorationtext.h"
#include "../../../tools/settings/options/gui/groupbox/settingsgroupbox.h"
#include "../../../tools/settings/options/gui/radiobutton/settingsradiobutton.h"


DEFAULT_PLUGIN_NS_BEGIN

class Settings : public ::Tools::Settings::Page
{
	Q_DECLARE_TR_FUNCTIONS(Settings)

public:
	class FileIcon : public ::Tools::Settings::GroupBox
	{
		Q_DECLARE_TR_FUNCTIONS(FileIcon)

	public:
		FileIcon(Option *parent);

		const ::Tools::Settings::RadioButton &onlyAppIcon() const { return m_onlyAppIcon; }
		const ::Tools::Settings::RadioButton &onlyTypeIcon() const { return m_onlyTypeIcon; }
		const ::Tools::Settings::RadioButton &appIconIfNoTypeIcon() const { return m_appIconIfNoTypeIcon; }

	private:
		::Tools::Settings::RadioButton m_onlyAppIcon;
		::Tools::Settings::RadioButton m_onlyTypeIcon;
		::Tools::Settings::RadioButton m_appIconIfNoTypeIcon;
	};

public:
	Settings(Option *parent);

	static Settings *instance();

	const FileIcon &fileIcon() const { return m_fileIcon; }

private:
	::Tools::Settings::DecorationText m_text;
	FileIcon m_fileIcon;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_PLUGINSETTINGS_H_ */
