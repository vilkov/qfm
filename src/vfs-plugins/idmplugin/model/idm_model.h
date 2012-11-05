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
#ifndef IDM_MODEL_H_
#define IDM_MODEL_H_

#include <tools/models/tree/treemodel.h>
#include "items/idm_item.h"


IDM_PLUGIN_NS_BEGIN

class Model : public ::Tools::Models::Tree::Model
{
public:
	Model(QObject *parent = 0);

protected:
	class Container : public ::Tools::Models::Tree::Model::Container
	{
	public:
		typedef QList<IdmItem *> List;

	public:
		Container();
		virtual ~Container();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

	private:
		friend class Model;
		friend class RootNode;
		friend class QueryResultsNode;
		List m_container;
	};

protected:
	Container m_container;
	Container::List &m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_MODEL_H_ */
