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
#ifndef IDM_PLUGINSETTINGS_H_
#define IDM_PLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include <tools/settings/options/gui/page/settingspage.h>
#include "../gui/query/create/settings/idm_createquerydialogsettings.h"
#include "../idm_ns.h"


IDM_PLUGIN_NS_BEGIN

class Settings : public ::Tools::Settings::Page
{
	Q_DECLARE_TR_FUNCTIONS(Settings)

public:
	Settings(Option *parent);

    static Settings *instance();

    const CreateQueryDialogSettings &createQueryDialog() const { return m_createQueryDialog; }
    CreateQueryDialogSettings &createQueryDialog() { return m_createQueryDialog; }

private:
    CreateQueryDialogSettings m_createQueryDialog;
};

IDM_PLUGIN_NS_END

#endif /* IDM_PLUGINSETTINGS_H_ */
