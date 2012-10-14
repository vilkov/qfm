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
#ifndef DEFAULT_ROOTNODEITEM_H_
#define DEFAULT_ROOTNODEITEM_H_

#include "default_nodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

class RootNodeItem : public NodeItem
{
public:
	RootNodeItem(Base *parent = 0);

	/* ::Tools::Models::Tree::Item */
	virtual QVariant data(qint32 column, qint32 role) const;

	virtual bool isRootItem() const;

	const QVariant &label() const { return m_label; }

private:
	QVariant m_label;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_ROOTNODEITEM_H_ */
