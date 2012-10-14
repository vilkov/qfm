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
#ifndef IDM_GROUPCONSTRAINT_H_
#define IDM_GROUPCONSTRAINT_H_

#include <QtCore/QCoreApplication>
#include "../idm_baseconstraint.h"


IDM_PLUGIN_NS_BEGIN

class GroupConstraint : public BaseConstraint
{
	Q_DECLARE_TR_FUNCTIONS(GroupConstraint)

public:
	typedef QList<BaseConstraint*> Container;
	typedef Container::size_type   size_type;
	enum { InvalidIndex = (size_type)-1 };
	enum Type
	{
		And,
		Or
	};

public:
	GroupConstraint(Type type, BaseConstraint *parent = 0);
	virtual ~GroupConstraint();

	/* BaseConstraint */
	virtual bool isGroup() const;
	virtual QString toString() const;

	static QString typeToString(Type type);

	Type type() const { return m_type; }
	void setType(Type type) { m_type = type; }

	size_type size() const { return m_items.size(); }
	BaseConstraint *at(size_type index) const { return m_items.at(index); }
	size_type indexOf(BaseConstraint *item) const { return m_items.indexOf(item); }

	void add(BaseConstraint *constraint);
	void insert(size_type index, BaseConstraint *constraint);
	void swap(size_type index1, size_type index2);
	void remove(size_type index);

protected:
	Type m_type;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_GROUPCONSTRAINT_H_ */
