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
#ifndef SETTINGSSCOPE_H_
#define SETTINGSSCOPE_H_

#include "settingslist.h"


SETTINGS_NS_BEGIN

class Scope : public List
{
public:
	Scope(const Id &id, Option *parent) :
		List(id, parent)
	{}

	void manage(Option *option);

protected:
	virtual void save(QXmlStreamWriter &stream) const;
	virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();
};

SETTINGS_NS_END

#endif /* SETTINGSSCOPE_H_ */
