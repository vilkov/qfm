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
#ifndef IDM_QUERYRESULTS_NODE_H_
#define IDM_QUERYRESULTS_NODE_H_

#include <vfs/tasks/vfs_tasksnode.h>
#include <tools/containers/union.h>
#include "idm_queryresultsdelegate.h"
#include "interface/idm_inodequeryresults.h"
#include "../../containeres/idm_container.h"
#include "../../storage/values/idm_valuereader.h"


IDM_PLUGIN_NS_BEGIN
class Functor;
class QueryResultValueItem;


class QueryResultsNode : public TasksNode, public IQueryResultsUpdater
{
public:
	QueryResultsNode(const IdmContainer &container, const Select &query, Node *parent = 0);

	/* QObject */
    virtual bool event(QEvent *event);

	/* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
    virtual void fetchMore(const QModelIndex &parent);
    virtual bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void contextMenu(const QModelIndexList &list, INodeView *view);
	virtual void cancel(const QModelIndexList &list, INodeView *view);

	/* INode */
    virtual void refresh();
	virtual QString title() const;
	virtual QString location() const;
	virtual QString fileName(const QModelIndex &index) const;
	virtual bool shortcut(INodeView *view, QKeyEvent *event);

	virtual Sorting sorting() const;
	virtual Geometry geometry() const;
	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;
	virtual void menuAction(INodeView *view, QAction *action);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual QModelIndex childIndex(const QString &fileName);
	virtual Node *viewChild(const QModelIndex &idx, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, QModelIndex &selected);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const Item::Holder &item, quint64 timeElapsed);
	virtual void performActionEvent(const AsyncAction::FilesList &files, const QString &error);

protected:
	/* IQueryResultsUpdater */
	virtual void add(const QModelIndex &index, const CompositeEntityValue::List &values);
	virtual void remove(const QModelIndex &index, const CompositeEntityValue::List &values);
	virtual void refresh(const QModelIndex &index);

protected:
	virtual void create(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndex &index, INodeView *view);
	virtual void remove(const QModelIndexList &list, INodeView *view);

private:
	void process(const QModelIndexList &list, const Functor &functor);
	void processRemove(const QModelIndexList &list, const Functor &functor);
	void doRename(INodeView *view, const QModelIndex &index, QueryResultValueItem *value);
	void doRemove(INodeView *view, const QModelIndex &index, QueryResultValueItem *value);

private:
	typedef ::Tools::Containers::Union Union;

	void scanUpdates(const BaseTask::Event *e);
	void scanForRemove(const BaseTask::Event *e);
	void performRemove(const BaseTask::Event *e);

private:
    enum { PrefetchLimit = 64 };

private:
	class ItemsContainer : public Container
	{
	public:
		typedef QList<VFS::Item::Holder> List;

	public:
		ItemsContainer();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

	private:
		friend class QueryResultsNode;
		List m_container;
	};

private:
	void lock(const Snapshot &snapshot, const QString &reason);
	void unlock(const Snapshot &snapshot);
	void update(Snapshot &updates);

private:
	enum ShortcutType
	{
		NoShortcut,

		CreateShortcut,
		RenameShortcut,
		RemoveShortcut,

		SizeOf_ShortcutType
	};

	typedef QMap<quint32, ShortcutType> Shortcuts;

private:
    ItemsContainer m_itemsContainer;
    ItemsContainer::List &m_items;
    INodeView::MenuActionList m_actions;
	QueryResultsDelegate m_delegate;
	IdmContainer m_container;
	ValueReader m_reader;
	Shortcuts m_shortcuts;
	QVariant m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTS_NODE_H_ */
