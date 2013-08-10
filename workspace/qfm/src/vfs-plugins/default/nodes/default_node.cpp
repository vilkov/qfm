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
#include "default_node.h"
#include "../model/items/default_rootnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

Node::Node(IFileContainer::Holder &container, ::VFS::Node *parent) :
	BaseNode(container, parent)
{
	RootNodeItem::Holder item(new RootNodeItem());
	items().add(item.as<RootNodeItem>()->label().toString(), item);
}

QModelIndex Node::rootIndex() const
{
	return proxy().mapFromSource(createIndex(0, 0, items()[0].data()));
}

Snapshot Node::updateFilesList() const
{
	Snapshot::Files files(container().data());

	for (Container::size_type i = 1, size = items().size(); i < size; ++i)
		files.add(items()[i].as<NodeItem>()->info()->fileName(), items()[i]);

	return files;
}

DEFAULT_PLUGIN_NS_END
