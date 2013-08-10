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
#include "settingsscope.h"


SETTINGS_NS_BEGIN

void Scope::manage(Option *option)
{
	m_items.push_back(&option->id(), option);
}

void Scope::save(QXmlStreamWriter &stream) const
{
	stream.writeStartElement(id());

	for (Container::const_iterator i = begin(), end = Scope::end(); i != end; ++i)
		(*i)->save(stream);

	stream.writeEndElement();
}

void Scope::load(QXmlStreamReader &stream)
{
	Option *option;
	List::Container uninitialized(m_items);

	while (readNextStartElement(stream))
		if (option = uninitialized.value(stream.name()))
		{
			option->load(stream);
			uninitialized.remove(&option->id());
		}

	for (List::Container::const_iterator i = uninitialized.constBegin(), end = uninitialized.constEnd(); i != end; ++i)
		(*i)->loadDefault();
}

void Scope::loadDefault()
{
	for (Container::const_iterator i = begin(), end = Scope::end(); i != end; ++i)
		(*i)->loadDefault();
}

SETTINGS_NS_END
