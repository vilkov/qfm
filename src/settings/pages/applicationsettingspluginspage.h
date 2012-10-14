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
#ifndef APPLICATIONSETTINGSPLUGINSPAGE_H_
#define APPLICATIONSETTINGSPLUGINSPAGE_H_

#include <QtCore/QCoreApplication>
#include "../../tools/settings/options/gui/page/settingspage.h"
#include "../../tools/settings/options/gui/decoration/settingsdecorationtext.h"


SETTINGS_NS_BEGIN

class PluginsPage : public Page
{
	Q_DECLARE_TR_FUNCTIONS(PluginsPage)

public:
	PluginsPage(Option *parent);

private:
	DecorationText m_text;
};

SETTINGS_NS_END

#endif /* APPLICATIONSETTINGSPLUGINSPAGE_H_ */
