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
#ifndef IDM_ROOTNODEFILESITEM_H_
#define IDM_ROOTNODEFILESITEM_H_

#include <QtCore/QCoreApplication>
#include <vfs/vfs_node.h>
#include "idm_rootnodeitem.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeFilesItem : public RootNodeItem
{
	Q_DECLARE_TR_FUNCTIONS(RootNodeFilesItem)

public:
	RootNodeFilesItem();

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* IRootNodeItem */
	virtual bool isFiles();

	Node *node() const { return m_node; }
	void setNode(Node *node) { m_node = node; }

private:
	QVariant m_label;
	Node *m_node;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ROOTNODEFILESITEM_H_ */
