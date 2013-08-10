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
#ifndef SETTINGSVISIBLEOPTIONHOTKEY_H_
#define SETTINGSVISIBLEOPTIONHOTKEY_H_

#include "../../settingsscope.h"
#include "../../value/settingsoptionvalue.h"


SETTINGS_NS_BEGIN

class VisibleOptionHotKey : public Scope
{
public:
	VisibleOptionHotKey(const QString &label, const QString &context, const QString &id, Option *parent, int modifiers = Qt::NoModifier, int key = 0);

	const QString &label() const { return m_label; }
	const QString &context() const { return m_context; }

	int modifiers() const { return m_modifiers.value().toInt(); }
	void setModifiers(int value) { m_modifiers.setValue(QString::number(value)); }

	int key() const { return m_key.value().toInt(); }
	void setKey(int value) { m_key.setValue(QString::number(value)); }

private:
	QString m_label;
	QString m_context;
	OptionValue m_modifiers;
	OptionValue m_key;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONHOTKEY_H_ */
