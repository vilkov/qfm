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
#ifndef SETTINGSOPTIONSLIST_H_
#define SETTINGSOPTIONSLIST_H_

#include "settingslist.h"


SETTINGS_NS_BEGIN

class OptionList : public List
{
public:
	OptionList(const Id &id, Option *parent) :
		List(id, parent)
	{}
	virtual ~OptionList();

	void add(Option *option) { m_items.push_back(&option->id(), option); }
	void remove(const Id &id) { delete m_items.take(&id); }
	void clear();

protected:
	virtual void save(QXmlStreamWriter &stream) const;
	virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();

protected:
	virtual bool isSubOptionName(const QStringRef &name) const = 0;
	virtual Option *create() = 0;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTIONSLIST_H_ */
