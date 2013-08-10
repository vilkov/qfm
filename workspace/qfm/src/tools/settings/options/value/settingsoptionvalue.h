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
#ifndef SETTINGSOPTIONVALUE_H_
#define SETTINGSOPTIONVALUE_H_

#include <QtCore/QString>
#include "../settingsoption.h"


SETTINGS_NS_BEGIN

class OptionValue : public Option
{
public:
	OptionValue(const Id &id, Option *parent, const QString &defaultValue = QString()) :
		Option(id, parent),
		m_defaultValue(defaultValue)
	{}

	const QString &value() const { return m_value; }
	void setValue(const QString &value) { m_value = value; }

protected:
    virtual void save(QXmlStreamWriter &stream) const;
    virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();

protected:
    const QString &defaultValue() const { return m_defaultValue; }

private:
    QString m_value;
    QString m_defaultValue;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTIONVALUE_H_ */
