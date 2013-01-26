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
#include "settingsoptionlist.h"
#include "../../memory/memory_scopedpointer.h"


SETTINGS_NS_BEGIN

OptionList::~OptionList()
{
	clear();
}

void OptionList::clear()
{
	qDeleteAll(m_items);
	m_items.clear();
}

void OptionList::save(QXmlStreamWriter &stream) const
{
	stream.writeStartElement(id());

	for (const_iterator i = begin(), end = List::end(); i != end; ++i)
		(*i)->save(stream);

	stream.writeEndElement();
}

void OptionList::load(QXmlStreamReader &stream)
{
	::Tools::Memory::ScopedPointer<Option> option;

	for (QXmlStreamReader::TokenType token = stream.readNext(); !stream.atEnd(); token = stream.readNext())
		if (token == QXmlStreamReader::EndElement && stream.name() == id())
			break;
		else
			if (token == QXmlStreamReader::StartElement && isSubOptionName(stream.name()))
			{
				option = create();
				option->load(stream);
				add(option.take());
			}
}

void OptionList::loadDefault()
{}

SETTINGS_NS_END
