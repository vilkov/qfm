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
#ifndef IDM_FOLDERNODE_H_
#define IDM_FOLDERNODE_H_

#include "../../idm_ns.h"
#include "../../containeres/idm_container.h"
#include "../../../default/nodes/default_node.h"


IDM_PLUGIN_NS_BEGIN

class FolderNode : public Default::Node
{
public:
    FolderNode(IFileContainer::Holder &container, const IdmContainer &storage, ::VFS::Node *parent = 0);

    /* IFileOperations */
    virtual ICopyControl *createControl(INodeView *view) const;

protected:
    /* Default::BaseNode */
    virtual void rename(const QModelIndex &index, INodeView *view);
    virtual void rename(const QModelIndexList &list, INodeView *view);
    virtual void remove(const QModelIndexList &list, INodeView *view);
    virtual void move(const INodeView *source, INodeView *destination);
    virtual void removeToTrash(const QModelIndexList &list, INodeView *view);

protected:
    virtual ::VFS::Node *createNode(const IFileInfo *file) const;

private:
    friend class RootNode;
    ::VFS::Node *privateViewChild(const QString &fileName, QModelIndex &selected);

private:
    IdmContainer m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDM_FOLDERNODE_H_ */
