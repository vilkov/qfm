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
#include "configoptionconstrainturl.h"

#include <QtCore/QUrl>
#include <QtGui/QMessageBox>


SETTINGS_NS_BEGIN

ConfigOptionConstraintUrl::ConfigOptionConstraintUrl(ConfigOptionValue *option) :
	m_option(option)
{}

bool ConfigOptionConstraintUrl::check(QWidget *parent) const
{
	if (!QUrl(m_option->editorValue().toString()).isValid())
	{
		QMessageBox::warning(parent, tr("Invalid value"), tr("Field \"%1\" value is not valid.").arg(m_option->title()));
		return false;
	}
	else
		return true;
}

SETTINGS_NS_END
