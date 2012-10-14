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
#include "idm_shortformat.h"


IDM_PLUGIN_NS_BEGIN

ShortFormat::ShortFormat(const QString &format) :
	m_format(format)
{
	QString token;
	token.reserve(m_format.size());

	for (size_type pos = 0; pos < m_format.size(); ++pos)
		if (m_format.at(pos) == QChar(L'$'))
			dollarToken(pos, token, m_format);
		else
			token.append(m_format.at(pos));

	if (!token.isEmpty())
	{
		token.truncate(token.size());
		m_items.push_back(Token(Token::Text, token));
	}
}

bool ShortFormat::isValid() const
{
	return !m_items.isEmpty();
}

void ShortFormat::dollarToken(size_type &pos, QString &token, const QString &source)
{
	if (pos + 1 < source.size())
		if (source.at(pos + 1) == QChar(L'{'))
			nameToken(++pos, token, source);
		else
			token.append(source.at(pos));
	else
		token.append(source.at(pos));
}

void ShortFormat::nameToken(size_type &pos, QString &token, const QString &source)
{
	if (pos + 2 < source.size())
	{
		QString name;

		for (++pos; pos < source.size(); ++pos)
			if (source.at(pos) == QChar(L'}'))
			{
				if (!name.isEmpty())
				{
					token.truncate(token.size());
					m_items.push_back(Token(Token::Text, token));
					m_items.push_back(Token(Token::Property, name));

					token.clear();
					token.reserve(source.size());
				}

				break;
			}
			else
				name.append(source.at(pos));
	}
	else
		token.append(source.at(pos));
}

IDM_PLUGIN_NS_END
