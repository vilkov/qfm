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
#ifndef SETTINGSOPTIONFONT_H_
#define SETTINGSOPTIONFONT_H_

#include <QtGui/QFont>
#include "settingsoptionvalue.h"
#include "../settingsscope.h"


SETTINGS_NS_BEGIN

class OptionFont : public Scope
{
public:
	OptionFont(const QString &id, Option *parent, const QFont &defaultValue);

	QFont font() const;
	void setFont(const QFont &font);

protected:
    virtual void loadDefault();

protected:
    const QFont &defaultValue() const { return m_defaultValue; }

    static QString toString(const QFont &font);

private:
    QFont m_defaultValue;
	OptionValue m_family;
	OptionValue m_styleName;
	OptionValue m_pointSize;
	OptionValue m_underline;
	OptionValue m_strikeOut;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTIONFONT_H_ */
