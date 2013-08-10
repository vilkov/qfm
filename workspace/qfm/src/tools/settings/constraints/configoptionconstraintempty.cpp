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
#include "configoptionconstraintempty.h"

#include <QtGui/QMessageBox>


SETTINGS_NS_BEGIN

ConfigOptionConstraintEmpty::ConfigOptionConstraintEmpty(ConfigOptionValue *option) :
	m_option(option)
{
	Q_ASSERT_X(m_option != 0, "ConfigOptionConstraintEmpty::ConfigOptionConstraintEmpty", "option == 0");
}

bool ConfigOptionConstraintEmpty::check(QWidget *parent) const
{
	if (m_option->editorValue().isNull())
	{
		QMessageBox::warning(parent, tr("Invalid value"), tr("Field \"%1\" can not be empty.").arg(m_option->title()));
		return false;
	}
	else
		return true;
}

SETTINGS_NS_END
