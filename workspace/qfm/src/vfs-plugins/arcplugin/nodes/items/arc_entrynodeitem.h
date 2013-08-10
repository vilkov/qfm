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
#ifndef ARC_ENTRYNODEITEM_H_
#define ARC_ENTRYNODEITEM_H_

#include <vfs/filetypeinfo/vfs_filetypeinfo.h>
#include <vfs/containers/vfs_snapshotitem.h>
#include "arc_nodeitem.h"


ARC_PLUGIN_NS_BEGIN

class EntryNodeItem : public NodeItem
{
public:
	typedef QList<Holder> Container;

public:
	EntryNodeItem(SnapshotItem *item, Base *parent = 0);

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;
	virtual QVariant data(qint32 column, qint32 role) const;

	/* ArcNodeItem */
	virtual void lock(const QString &reason);

	const IFileInfo::Holder &info() const { return m_info; }

private:
	Container m_items;
	IFileInfo::Holder m_info;
	IFileInfo::size_type m_totalSize;
};

ARC_PLUGIN_NS_END

#endif /* ARC_ENTRYNODEITEM_H_ */
