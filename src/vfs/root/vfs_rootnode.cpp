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
#include "vfs_rootnode.h"
#include "../vfs_node.h"


VFS_NS_BEGIN

RootNode::RootNode()
{}

RootNode::~RootNode()
{}

::History::Entry *RootNode::open(INodeView *nodeView, const QString &uri, const QString &currentFile) const
{
	Uri path(uri);

	if (path.isValid() && !path.isEmpty())
	{
		QModelIndex selected;
		IContainerPlugin *plugin;

		if (path.shema().isEmpty() ||
			(plugin = m_containerPlugins.value(path.shema())) == NULL)
		{
			static const QString shema(QString::fromLatin1("file"));
			plugin = m_containerPlugins.value(shema);
		}

		if (Node *node = plugin->open(path.begin(), selected))
		{
			/* XXX: Add 2 links because of HistoryEntry */
			if (selected.isValid() || currentFile.isEmpty())
				node->viewThis(nodeView, selected, 2);
			else
				node->viewThis(nodeView, currentFile, 2);

			node->refresh();

			return new Node::HistoryEntry(node);
		}
	}

	return NULL;
}

Node *RootNode::open(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	Node *res;
	const PluginsList list = m_fileTypePlugins.value(file->fileType()->id());

	for (PluginsList::size_type i = 0, size = list.size(); i < size; ++i)
		if (res = list.at(i)->open(container, file, parent))
			return res;

	return NULL;
}

void RootNode::registerStatic(IFilePlugin *plugin)
{
	IFilePlugin::FileTypeIdList types = plugin->fileTypes();

	for (IFilePlugin::FileTypeIdList::const_iterator i = types.constBegin(), end = types.constEnd(); i != end; ++i)
		m_fileTypePlugins[*i].push_back(plugin);

	plugin->registered();
}

void RootNode::registerStatic(IContainerPlugin *plugin)
{
	m_containerPlugins[plugin->shema()] = plugin;
}

VFS_NS_END
