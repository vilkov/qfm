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
#ifndef VFS_ROOTNODE_H_
#define VFS_ROOTNODE_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "../interfaces/vfs_inodeview.h"
#include "../interfaces/vfs_iplugin.h"
#include "../../history/historyentry.h"


VFS_NS_BEGIN

class RootNode
{
	Q_DISABLE_COPY(RootNode)

public:
	RootNode();
	virtual ~RootNode();

	void container(const QString &uri, IFileContainer::Holder &container, IFileInfo::Holder &info, QString &error) const;
	::History::Entry *open(INodeView *nodeView, const QString &uri, const QString &currentFile = QString()) const;
	Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

protected:
	void registerStatic(IFilePlugin *plugin);
	void registerStatic(IContainerPlugin *plugin);

private:
	typedef QList<IFilePlugin *>              PluginsList;
	typedef QMap<FileTypeId, PluginsList>     PluginsMap;
	typedef QMap<QString, IContainerPlugin *> ContentPluginsMap;

private:
	PluginsMap m_fileTypePlugins;
	ContentPluginsMap m_containerPlugins;
};

VFS_NS_END

#endif /* VFS_ROOTNODE_H_ */
