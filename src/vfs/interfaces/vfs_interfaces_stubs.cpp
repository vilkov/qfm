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
#include "vfs_inode.h"
#include "vfs_iplugin.h"
#include "vfs_inodeview.h"
#include "vfs_ifileinfo.h"
#include "vfs_ifiletype.h"
#include "vfs_iapplications.h"
#include "vfs_ifilecontainer.h"
#include "vfs_ifileoperations.h"
#include "vfs_ifilenavigation.h"
#include "vfs_ifilecontainerscanner.h"

VFS_NS_BEGIN

INode::~INode()
{}

IPlugin::~IPlugin()
{}

INodeView::~INodeView()
{}

IFileInfo::~IFileInfo()
{}

IFileType::~IFileType()
{}

IApplication::~IApplication()
{}

IApplications::~IApplications()
{}

IFileContainer::~IFileContainer()
{}

IFileOperations::~IFileOperations()
{}

IFileNavigation::~IFileNavigation()
{}

IFileContainerScanner::Filter::~Filter()
{}

IFileContainerScanner::~IFileContainerScanner()
{}

IFileContainerScanner::IEnumerator::~IEnumerator()
{}

VFS_NS_END
