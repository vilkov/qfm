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
#ifndef SETTINGSOPTIONVALUELIST_H_
#define SETTINGSOPTIONVALUELIST_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include "../settingsoption.h"


SETTINGS_NS_BEGIN

class OptionValueList : public Option
{
public:
	typedef QList<QString>       Container;
	typedef Container::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	OptionValueList(const Id &id, Option *parent) :
		Option(id, parent)
	{}

	QString at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(const QString &item) const { return m_items.indexOf(item); }

	void add(const QString &value) { m_items.push_back(value); }
	void remove(size_type index) { m_items.removeAt(index); }
	void clear() { m_items.clear(); }

protected:
    virtual void save(QXmlStreamWriter &stream) const;
    virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();

protected:
	Container m_items;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTIONVALUELIST_H_ */
