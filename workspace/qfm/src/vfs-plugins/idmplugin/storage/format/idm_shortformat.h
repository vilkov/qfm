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
#ifndef IDM_SHORTFORMAT_H_
#define IDM_SHORTFORMAT_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include "../../idm_ns.h"


IDM_PLUGIN_NS_BEGIN

class ShortFormat
{
public:
	class Token
	{
	public:
		enum Type
		{
			Text,
			Property
		};

	public:
		Token(Type type, const QString &string) :
			m_type(type),
			m_string(string)
		{}

		Type type() const { return m_type; }
		const QString &string() const { return m_string; }

	private:
		Type m_type;
		QString m_string;
	};

	typedef QList<Token>         Container;
	typedef Container::size_type size_type;

public:
	ShortFormat(const QString &format);

	bool isValid() const;
	const QString &format() const { return m_format; }
	const QString &lastError() const { return m_lastError; }

	size_type size() const { return m_items.size(); }
	const Token &at(size_type index) const { return m_items.at(index); }

private:
	void dollarToken(size_type &pos, QString &token, const QString &source);
	void nameToken(size_type &pos, QString &token, const QString &source);

private:
	QString m_format;
	Container m_items;
	QString m_lastError;
};

IDM_PLUGIN_NS_END

#endif /* IDM_SHORTFORMAT_H_ */
