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
#ifndef SETTINGSDIALOGSETTINGS_H_
#define SETTINGSDIALOGSETTINGS_H_

#include <QtGui/QFont>
#include "../../options/settingswidgetscope.h"


SETTINGS_NS_BEGIN

class DialogSettings : public WidgetScope
{
public:
	DialogSettings(Option *parent);

	virtual QFont font() const = 0;

	QByteArray splitterState() const { return QByteArray::fromBase64(m_splitterState.value().toAscii()); }
	void setSplitterState(const QByteArray &value) { m_splitterState.setValue(QString::fromAscii(value.toBase64())); }

private:
	OptionValue m_splitterState;
};

SETTINGS_NS_END

#endif /* SETTINGSDIALOGSETTINGS_H_ */
