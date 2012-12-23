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
#ifndef IDM_QUERYRESULTPATHITEM_H_
#define IDM_QUERYRESULTPATHITEM_H_

#include <vfs/vfs_node.h>
#include "idm_queryresultitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPathItem : public QueryResultItem
{
public:
	typedef QList<QueryResultItem::Holder> Container;

public:
	QueryResultPathItem(IFileInfo::Holder &info, Base *parent);
	QueryResultPathItem(const IFileContainer *container, const QString &fileName, Base *parent);

	/* Base */
	virtual Base *at(Base::size_type index) const;
	virtual Base::size_type size() const;
	virtual Base::size_type indexOf(Base *item) const;
	virtual QVariant data(qint32 column, qint32 role) const;

	/* QueryResultItem */
	virtual bool isRoot();
	virtual bool isProperty();
	virtual bool isValue();
	virtual bool isPath();

	const IFileInfo::Holder &info() const { return m_info; }
	IFileInfo::Holder &info() { return m_info; }
	const Location location() const { return m_location; }

	virtual void open() const = 0;

	Node *node() const { return m_node; }
	void setNode(Node *node) { m_node = node; }

protected:
	IFileInfo::Holder m_info;
    Location m_location;
	Node *m_node;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTPATHITEM_H_ */
