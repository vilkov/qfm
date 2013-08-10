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
#include "settingsvisibleoptionhotkey.h"


SETTINGS_NS_BEGIN

VisibleOptionHotKey::VisibleOptionHotKey(const QString &label, const QString &context, const QString &id, Option *parent, int modifiers, int key) :
	Scope(id, parent),
	m_label(label),
	m_context(context),
	m_modifiers(QString::fromLatin1("Modifiers"), this, QString::number(modifiers)),
	m_key(QString::fromLatin1("Key"), this, QString::number(key))
{
	manage(&m_modifiers);
	manage(&m_key);
}

SETTINGS_NS_END
