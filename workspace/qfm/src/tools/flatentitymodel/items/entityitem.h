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
#ifndef ENTITYITEM_H_
#define ENTITYITEM_H_

#include <QtCore/QList>
#include <QtCore/QVariant>


class EntityItem
{
public:
    typedef QList<EntityItem*>    value_type;
	typedef value_type::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	virtual ~EntityItem() {}

	virtual EntityItem *parent() const { return 0; }
	virtual EntityItem *child(size_type index) const { return 0; }
	virtual size_type size() const { return 0; }
	virtual size_type indexOf(EntityItem *item) const { return InvalidIndex; }
	virtual QVariant data(qint32 column, qint32 role) const = 0;
};

#endif /* ENTITYITEM_H_ */
