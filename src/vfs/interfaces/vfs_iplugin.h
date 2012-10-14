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
#ifndef VFS_IPLUGIN_H_
#define VFS_IPLUGIN_H_

#include <QtCore/QModelIndex>
#include "vfs_ifilecontainer.h"
#include "../tools/vfs_uri.h"
#include "../../tools/settings/options/gui/page/settingspage.h"


VFS_NS_BEGIN
class Node;


class IPlugin
{
public:
	virtual ~IPlugin();

	virtual void registered() = 0;
	virtual ::Tools::Settings::Page *settings() = 0;
};


class IContainerPlugin : public virtual IPlugin
{
public:
	virtual QString shema() const = 0;
	virtual Node *open(const Uri::Iterator &path, QModelIndex &selected) const = 0;
};


class IFilePlugin : public virtual IPlugin
{
public:
	typedef QList<FileTypeId> FileTypeIdList;

public:
	virtual FileTypeIdList fileTypes() const = 0;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const = 0;
};

VFS_NS_END

#endif /* VFS_IPLUGIN_H_ */
