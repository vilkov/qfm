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
#ifndef M3UNODE_H_
#define M3UNODE_H_

#include <QtCore/QFileInfo>
#include <vfs/vfs_node.h>
#include "m3uplugin_ns.h"
#include "m3udelegate.h"
#include "m3uproxymodel.h"
#include "items/m3uitem.h"


M3U_PLUGIN_NS_BEGIN

class M3uNode : public Node
{
public:
	M3uNode(const QFileInfo &info, Node *parent = 0);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* IFileOperations */
	virtual QString fileName(const QModelIndex &idx) const;
	virtual IFileInfo *info(const QModelIndex &idx) const;
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void contextMenu(const QModelIndexList &list, INodeView *view);
	virtual void createFile(const QModelIndex &index, INodeView *view);
	virtual void createDirectory(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndexList &list, INodeView *view);
	virtual void remove(const QModelIndexList &list, INodeView *view);
	virtual void cancel(const QModelIndexList &list, INodeView *view);
	virtual void calculateSize(const QModelIndexList &list, INodeView *view);
	virtual void pathToClipboard(const QModelIndexList &list, INodeView *view);
	virtual void copy(const INodeView *source, INodeView *destination);
	virtual void move(const INodeView *source, INodeView *destination);
	virtual void removeToTrash(const QModelIndexList &list, INodeView *view);
	virtual ::History::Entry *search(const QModelIndex &index, INodeView *view);

	/* INode */
    virtual void refresh();
	virtual QString title() const;
	virtual QString location() const;
	virtual bool shortcut(INodeView *view, QKeyEvent *event);

	virtual void menuAction(INodeView *view, QAction *action);

protected:
	virtual QModelIndex rootIndex() const { return QModelIndex(); }
	virtual QModelIndex childIndex(const QString &fileName) { return QModelIndex(); }
	virtual QAbstractItemModel *proxyModel() const { return &((M3uNode *)this)->m_proxy; }
	virtual QAbstractItemDelegate *itemDelegate() const { return &((M3uNode *)this)->m_delegate; }
	virtual const INodeView::MenuActionList &menuActions() const { return m_menuActions; }

	virtual Node *viewChild(const QModelIndex &idx, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, QModelIndex &selected);

protected:
	M3uItem *rootItem() const { return m_items.container().at(0); }
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

private:
	class Container : public Node::Container
	{
	public:
		typedef QList<M3uItem *> List;

	public:
		Container();
		virtual ~Container();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

		const List &container() const { return m_container; }
		List &container() { return m_container; }

	private:
		List m_container;
	};

private:
	QString m_tag;
	bool m_updating;
	Container m_items;
	M3uProxyModel m_proxy;
	M3uDelegate m_delegate;
	INodeView::MenuActionList m_menuActions;
};

M3U_PLUGIN_NS_END

#endif /* M3UNODE_H_ */
