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
#ifndef STRINGSHIERARCHYTREE_H_
#define STRINGSHIERARCHYTREE_H_

#include "stringshierarchyitem.h"


STRINGS_HIERARCHY_NS_BEGIN

class Tree : public Item
{
public:
	Tree(QChar delimeter) :
		Item(QString(), 0),
		m_delimeter(delimeter)
	{}

	QChar delimeter() const { return m_delimeter; }

	void add(const QString &string)
	{
		int index1 = 0;
		Item *parent = this;
		QString last;

		for (int index2 = string.indexOf(m_delimeter); index2 != -1; index2 = string.indexOf(m_delimeter, index1 = (++index2)))
			parent = parent->add(string.mid(index1, index2 - index1));

		if (!(last = string.mid(index1, string.size() - index1)).isEmpty())
			parent->add(last);
	}

	QString make(Item *item) const
	{
		QString res = item->string();

		for (item = item->parent(); item; item = item->parent())
			res.prepend(m_delimeter).prepend(item->string());

		return res.remove(0, 1);
	}

private:
	QChar m_delimeter;
};

STRINGS_HIERARCHY_NS_END

#endif /* STRINGSHIERARCHYTREE_H_ */
