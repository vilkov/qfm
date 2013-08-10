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
#ifndef SETTINGSLIST_H_
#define SETTINGSLIST_H_

#include "settingsoption.h"
#include "../../containers/orderedmap.h"


SETTINGS_NS_BEGIN

class List : public Option
{
public:
	typedef Containers::OrderedMap<QStringRef, Option *> Container;
	typedef Container::const_iterator                    const_iterator;

public:
	List(const Id &id, Option *parent = 0) :
		Option(id, parent)
	{}

	bool isEmpty() const { return m_items.isEmpty(); }

	Option *find(const Id &id) const { return m_items.value(&id); }
	const_iterator begin() const { return m_items.constBegin(); }
	const_iterator end() const { return m_items.constEnd(); }

protected:
	Container m_items;
};

SETTINGS_NS_END

#endif /* SETTINGSLIST_H_ */
