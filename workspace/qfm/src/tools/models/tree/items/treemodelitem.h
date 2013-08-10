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
#ifndef TREEMODELITEM_H_
#define TREEMODELITEM_H_

#include <QtCore/QVariant>
#include "../treemodel_ns.h"


MODELS_TREE_NS_BEGIN

class Item
{
public:
	typedef Item   Base;
	typedef size_t size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	Item(Base *parent);
	virtual ~Item();

	Base *parent() const { return m_parent; }

	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

	virtual QVariant data(qint32 column, qint32 role) const = 0;

private:
	Base *m_parent;
};

MODELS_TREE_NS_END

#endif /* TREEMODELITEM_H_ */
