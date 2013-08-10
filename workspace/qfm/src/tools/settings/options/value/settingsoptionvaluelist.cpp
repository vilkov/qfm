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
#include "settingsoptionvaluelist.h"


SETTINGS_NS_BEGIN

void OptionValueList::save(QXmlStreamWriter &stream) const
{
	static const QString id = QString::fromLatin1("Value");

	stream.writeStartElement(OptionValueList::id());

	for (size_type i = 0, size = OptionValueList::size(); i < size; ++i)
		stream.writeTextElement(id, at(i));

	stream.writeEndElement();
}

void OptionValueList::load(QXmlStreamReader &stream)
{
	for (QXmlStreamReader::TokenType token = stream.readNext(); !stream.atEnd(); token = stream.readNext())
		if (token == QXmlStreamReader::StartElement)
			m_items.push_back(stream.readElementText());
		else
			if (token == QXmlStreamReader::EndElement && stream.name() == id())
				break;
}

void OptionValueList::loadDefault()
{}

SETTINGS_NS_END
