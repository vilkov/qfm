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
#ifndef CONFIGOPTIONCONSTRAINTRANGE_H_
#define CONFIGOPTIONCONSTRAINTRANGE_H_

#include <QtCore/QCoreApplication>
#include "configoptionconstraint.h"
#include "../configoptions/configoptionvalue.h"


SETTINGS_NS_BEGIN

class ConfigOptionConstraintRange : public ConfigOptionConstraint
{
	Q_DECLARE_TR_FUNCTIONS(ConfigOptionConstraintRange)

public:
	ConfigOptionConstraintRange(ConfigOptionValue *option, const qint32 &min, const qint32 &max);
	virtual bool check(QWidget *parent) const;

private:
	ConfigOptionValue *m_option;
	qint32 m_min;
	qint32 m_max;
};

SETTINGS_NS_END

#endif /* CONFIGOPTIONCONSTRAINTRANGE_H_ */
