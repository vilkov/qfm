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
#include "configoptionconstraintrange.h"

#include <QtGui/QMessageBox>


SETTINGS_NS_BEGIN

ConfigOptionConstraintRange::ConfigOptionConstraintRange(ConfigOptionValue *option, const qint32 &min, const qint32 &max) :
	m_option(option),
	m_min(min),
	m_max(max)
{
	Q_ASSERT_X(m_min <= m_max, "ConfigOptionConstraintRange::ConfigOptionConstraintRange", "min > max");
}

bool ConfigOptionConstraintRange::check(QWidget *parent) const
{
	bool ok;
	qint32 val = m_option->editorValue().toInt(&ok);

	if (!ok || val < m_min || val > m_max)
	{
		QMessageBox::warning(parent, tr("Out of range"), tr("Field \"%1\" must be between %2 and %3.").arg(m_option->title()).arg(m_min).arg(m_max));
		return false;
	}
	else
		return true;
}

SETTINGS_NS_END
