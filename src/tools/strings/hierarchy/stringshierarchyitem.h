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
#ifndef STRINGSHIERARCHYITEM_H_
#define STRINGSHIERARCHYITEM_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include "stringshierarchy_ns.h"


STRINGS_HIERARCHY_NS_BEGIN

class Item
{
public:
	typedef QMap<QString, Item*>      Container;
	typedef Container::size_type      size_type;
	typedef Container::const_iterator const_iterator;
	enum { InvalidIndex = (size_type)-1 };

public:
	Item(const QString &string, Item *parent = 0) :
		m_parent(parent),
		m_string(string)
	{}

	Item *parent() const { return m_parent; }
	const QString &string() const { return m_string; }

	size_type size() const { return m_items.size(); }
	const_iterator begin() const { return m_items.constBegin(); }
	const_iterator end() const { return m_items.constEnd(); }

private:
	friend class Tree;
	Item *add(const QString &string)
	{
		if (Item *&res = m_items[string])
			return res;
		else
			return (res = new Item(string, this));
	}

private:
	Item *m_parent;
	QString m_string;
	Container m_items;
};

STRINGS_HIERARCHY_NS_END

#endif /* STRINGSHIERARCHYITEM_H_ */
