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
#ifndef CONFIGOPTION_H_
#define CONFIGOPTION_H_

#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../settings_ns.h"
#include "../../pointers/pscopedpointer.h"


SETTINGS_NS_BEGIN

class ConfigOption
{
public:
	enum Type
	{
		Value,
		Group
	};

public:
	ConfigOption(const QString &title, ConfigOption *parent = 0) :
		m_title(title),
		m_parent(parent)
	{}
	virtual ~ConfigOption() {}

	virtual Type type() const = 0;
	virtual bool isEnabled() const = 0;
	virtual void setEnabled(bool value) = 0;
	const QString &title() const { return m_title; }
	ConfigOption *parent() const { return m_parent; }

private:
	QString m_title;
	ConfigOption *m_parent;
};

SETTINGS_NS_END

#endif /* CONFIGOPTION_H_ */
