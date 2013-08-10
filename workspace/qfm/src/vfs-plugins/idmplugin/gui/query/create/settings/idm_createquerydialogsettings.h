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
#ifndef IDM_CREATEQUERYDIALOGSETTINGS_H_
#define IDM_CREATEQUERYDIALOGSETTINGS_H_

#include <tools/settings/options/settingssplittedwidgetscope.h>
#include "../../../../idm_ns.h"


IDM_PLUGIN_NS_BEGIN

class CreateQueryDialogSettings : public ::Tools::Settings::SplittedWidgetScope
{
public:
    CreateQueryDialogSettings(Option *parent);

    int column1() const { return m_column1.value().toInt(); }
    void setColumn1(int value) { m_column1.setValue(QString::number(value)); }

    int column2() const { return m_column2.value().toInt(); }
    void setColumn2(int value) { m_column2.setValue(QString::number(value)); }

private:
    ::Tools::Settings::OptionValue m_column1;
    ::Tools::Settings::OptionValue m_column2;
};

IDM_PLUGIN_NS_END

#endif /* IDM_CREATEQUERYDIALOGSETTINGS_H_ */
