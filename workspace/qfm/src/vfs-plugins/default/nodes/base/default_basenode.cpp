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
#include "default_basenode.h"
#include "default_basenode_p.h"
#include "../search/default_searchnode.h"
#include "../../search/dialog/default_searchdialog.h"
#include "../../tasks/scan/default_scanfilestask.h"
#include "../../tasks/scan/default_scanclipboardfilestask.h"
#include "../../tasks/perform/default_performcopytask.h"
#include "../../tasks/perform/default_performmovetask.h"
#include "../../tasks/perform/default_performremovetask.h"
#include "../../actions/default_globalactions.h"
#include "../../interfaces/default_invalidfileinfo.h"
#include "../../model/items/default_tmpnodeitem.h"

#include <application.h>
#include <vfs/filters/vfs_filters.h>
#include <vfs/contextmenu/vfs_contextmenu.h>
#include <vfs/tools/vfs_commontools.h>

#include <vfs/tasks/vfs_performactiontask.h>
#include <vfs/actions/synchronous/vfs_syncaction.h>
#include <vfs/actions/asynchronous/vfs_asyncaction.h>

#include <platform/platform.h>
#include <tools/containers/orderedmap.h>
#include <tools/widgets/stringdialog/stringdialog.h>

#include <QtGui/QMenu>
#include <QtGui/QClipboard>
#include <QtGui/QMessageBox>
#include <QtCore/QMimeData>


DEFAULT_PLUGIN_NS_BEGIN

static const QString uriListMimetype = QString::fromLatin1("text/uri-list");
#if PLATFORM_DE(KDE)
    static const QString kdeCutMimetype = QString::fromLatin1("application/x-kde-cutselection");
#endif


class ClipboardMimeData : public QMimeData
{
    Q_DISABLE_COPY(ClipboardMimeData)

public:
    ClipboardMimeData(bool move) :
        QMimeData(),
        m_move(move)
    {}

    void setData(const QByteArray &data)
    {
        QMimeData::setData(uriListMimetype, data);
    }

#if PLATFORM_DE(KDE)
    virtual bool hasFormat(const QString &mimetype) const
    {
        return (m_move && mimetype == kdeCutMimetype) || QMimeData::hasFormat(mimetype);
    }

    virtual QStringList formats() const
    {
        if (m_move)
            return QMimeData::formats() << kdeCutMimetype;

        return QMimeData::formats();
    }

protected:
    virtual QVariant retrieveData(const QString &mimetype, QVariant::Type preferredType) const
    {
        if (mimetype == kdeCutMimetype && m_move)
            return QByteArray("1");

        return QMimeData::retrieveData(mimetype, preferredType);
    }
#endif

private:
    bool m_move;
};


BaseNode::BaseNode(IFileContainer::Holder &container, Node *parent) :
	TasksNode(m_items, parent),
	m_container(container.take()),
	m_updating(false),
	m_proxy(this),
	m_delegate(&m_proxy)
{
    GlobalActions::instance()->addRef();

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);

	m_shortcuts[Qt::ALT + Qt::CTRL + Qt::Key_X]      = PathToClipboardShortcut;
	m_shortcuts[Qt::NoModifier     + Qt::Key_F2]     = RenameShortcut;
	m_shortcuts[Qt::NoModifier     + Qt::Key_F8]     = CreateFileShortcut;
	m_shortcuts[Qt::NoModifier     + Qt::Key_F7]     = CreateDirectoryShortcut;
	m_shortcuts[Qt::SHIFT          + Qt::Key_Delete] = RemoveShortcut;
	m_shortcuts[Qt::NoModifier     + Qt::Key_Space]  = CalculateSizeShortcut;
	m_shortcuts[Qt::NoModifier     + Qt::Key_F5]     = CopyShortcut;
	m_shortcuts[Qt::NoModifier     + Qt::Key_F6]     = MoveShortcut;
	m_shortcuts[Qt::CTRL           + Qt::Key_F]      = SearchShortcut;
    m_shortcuts[Qt::CTRL           + Qt::Key_C]      = CopyThroughClipboardShortcut;
    m_shortcuts[Qt::CTRL           + Qt::Key_X]      = CutThroughClipboardShortcut;
    m_shortcuts[Qt::CTRL           + Qt::Key_V]      = PasteThroughClipboardShortcut;
}

BaseNode::~BaseNode()
{
    GlobalActions::instance()->release();
}

int BaseNode::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant BaseNode::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		switch (section)
		{
			case 0:
			{
				return tr("Name");
				break;
			}
			case 1:
			{
				return tr("Size");
				break;
			}
			case 2:
			{
				return tr("Modified");
				break;
			}
		}

	return QVariant();
}

bool BaseNode::event(QEvent *e)
{
	switch (static_cast<FilesBaseTask::Event::Type>(e->type()))
	{
		case FilesBaseTask::Event::UpdateFiles:
		{
			e->accept();
			updateFiles(static_cast<BaseTask::Event *>(e));
			return true;
		}

		case FilesBaseTask::Event::ScanFilesForRemove:
		{
			e->accept();
			scanForRemove(static_cast<BaseTask::Event *>(e));
			return true;
		}

		case FilesBaseTask::Event::RemoveFiles:
		{
			e->accept();
			performRemove(static_cast<BaseTask::Event *>(e));
			return true;
		}

		case FilesBaseTask::Event::ScanFilesForSize:
		{
			e->accept();
			scanForSize(static_cast<BaseTask::Event *>(e));
			return true;
		}

		case FilesBaseTask::Event::ScanFilesForCopy:
		{
			e->accept();
			scanForCopy(static_cast<BaseTask::Event *>(e));
			return true;
		}

		case FilesBaseTask::Event::CopyFiles:
		{
			e->accept();
			performCopy(static_cast<BaseTask::Event *>(e));
			return true;
		}

        case FilesBaseTask::Event::ScanClipboardFiles_Update:
        {
            e->accept();
            scanClipboardFiles(static_cast<BaseTask::Event *>(e));
            return true;
        }

        default:
			return TasksNode::event(e);
	}
}

void BaseNode::refresh()
{
	if (!isUpdating())
		updateFiles();
}

QString BaseNode::title() const
{
	QString res = m_container->location();
	return res.mid(res.lastIndexOf(QChar(L'/')) + 1);
}

QString BaseNode::location() const
{
	return m_container->location();
}

QString BaseNode::fileName(const QModelIndex &index) const
{
	QModelIndex idx = m_proxy.mapToSource(index);

	if (idx.isValid() && !static_cast<NodeItem *>(idx.internalPointer())->isRootItem())
		return static_cast<NodeItem *>(idx.internalPointer())->info()->fileName();
	else
		return QString();
}

bool BaseNode::shortcut(INodeView *view, QKeyEvent *event)
{
	switch (m_shortcuts.value(event->modifiers() + event->key(), NoShortcut))
	{
		case CreateFileShortcut:
			createFile(view->currentIndex(), view);
			return true;

		case CreateDirectoryShortcut:
			createDirectory(view->currentIndex(), view);
			return true;

		case RenameShortcut:
			rename(view->currentIndex(), view);
			return true;

		case RemoveShortcut:
			remove(view->selectedIndexes(), view);
			return true;

		case CalculateSizeShortcut:
			calculateSize(view->selectedIndexes(), view);
			return true;

		case PathToClipboardShortcut:
			pathToClipboard(view->selectedIndexes(), view);
			return true;

		case CopyShortcut:
			copy(view, view->opposite());
			return true;

		case MoveShortcut:
			move(view, view->opposite());
			return true;

		case RemoveToTrashShortcut:
			removeToTrash(view->selectedIndexes(), view);
			return true;

		case SearchShortcut:
			search(view->currentIndex(), view);
			return true;

		case CopyThroughClipboardShortcut:
		    copyThroughClipboard(view, view->selectedIndexes(), false);
            return true;

		case CutThroughClipboardShortcut:
            copyThroughClipboard(view, view->selectedIndexes(), true);
            return true;

		case PasteThroughClipboardShortcut:
            pasteThroughClipboard(view);
            return true;

		default:
			return false;
	}
}

QAbstractItemModel *BaseNode::model() const
{
	return const_cast<ProxyModel *>(&m_proxy);
}

QAbstractItemDelegate *BaseNode::delegate() const
{
	return const_cast<Delegate *>(&m_delegate);
}

const INodeView::MenuActionList &BaseNode::actions() const
{
	return m_menuActions;
}

void BaseNode::menuAction(INodeView *view, QAction *action)
{

}

ICopyControl *BaseNode::createControl(INodeView *view) const
{
	return m_container->createControl(view);
}

void BaseNode::contextMenu(const QModelIndexList &list, INodeView *view)
{
	typedef QSet<NodeItem::Holder>                           ItemsSet;
	typedef QList<NodeItem::Holder>                          ItemsList;
	typedef QMap<NodeItem::Holder, ::Tools::Containers::Dot> ItemsIndexMap;

	ItemsSet set;
	ItemsList items;
	QModelIndex index;
	NodeItem::Holder item;
	ItemsIndexMap itemsIndex;
    ContextMenu contextMenu(m_container.data());

	for (ItemsList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!(item = m_items[(index = m_proxy.mapToSource(list.at(i))).row()]).as<NodeItem>()->isRootItem() && !set.contains(item))
		{
			set.insert(item);
			itemsIndex[item] = index.row();
			contextMenu.add(item, item.as<NodeItem>()->info());
		}

	items = set.toList();

	contextMenu.add(GlobalActions::instance()->copyAction(), ContextMenu::GeneralSection);
    contextMenu.add(GlobalActions::instance()->cutAction(), ContextMenu::GeneralSection);

	if (items.isEmpty())
	    contextMenu.add(GlobalActions::instance()->pasteClipboardAction(), ContextMenu::GeneralSection);
	else
		if (items.size() == 1)
			if ((item = items.at(0)).as<NodeItem>()->info()->isDir())
			    contextMenu.add(GlobalActions::instance()->pasteIntoFolderAction(), ContextMenu::GeneralSection);
			else
                contextMenu.add(GlobalActions::instance()->pasteAction(), ContextMenu::GeneralSection);
		else
            contextMenu.add(GlobalActions::instance()->pasteAction(), ContextMenu::GeneralSection);

    contextMenu.add(GlobalActions::instance()->propertiesAction(), ContextMenu::PropertiesSection);

	if (const Action *action = contextMenu.exec())
	{
		Action::FilesList files = contextMenu.files(action);

		if (action->isAsynchronous())
		{
			::Tools::Memory::ScopedPointer<PerformActionTask> task;

			if (task = static_cast<const AsyncAction *>(action)->process(this, m_container, files))
			{
				Union update;
				Snapshot::Files list(m_container);

				for (Action::FilesList::size_type i = 0, size = files.size(); i < size; ++i)
				{
					item = files.at(i).first;

					item->lock(static_cast<const AsyncAction *>(action)->lockReason());
					update.add(itemsIndex.value(item));
					list.add(item.as<NodeItem>()->info()->fileName(), item);
				}

				addTask(task.take(), list);
				updateFirstColumn(update);
			}
		}
		else
			static_cast<const SyncAction *>(action)->process(m_container, files);
	}
}

QModelIndex BaseNode::rootIndex() const
{
	return QModelIndex();
}

QModelIndex BaseNode::childIndex(const QString &fileName)
{
	Container::size_type index = m_items.indexOf(fileName);

	if (index == Container::InvalidIndex)
	{
		QString error;
		IFileInfo::Holder info;

		if (info = m_container->info(fileName, error))
		{
			NodeItem *item;

			beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
			m_items.add(NodeItem::Holder(item = new NodeItem(info)));
			endInsertRows();

			return indexForFile(item, m_items.lastIndex());
		}
	}
	else
		return indexForFile(m_items[index].as<NodeItem>(), index);

	return QModelIndex();
}

Node *BaseNode::viewChild(const QModelIndex &idx, QModelIndex &selected)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<NodeItem *>(index.internalPointer())->isRootItem())
		return parentNode();
	else
		if (!static_cast<NodeItem *>(index.internalPointer())->isLocked())
		{
			NodeItem *entry = static_cast<NodeItem *>(index.internalPointer());

			if (entry->node())
				return entry->node();
			else
				if (Node *node = createNode(entry->info()))
				{
					entry->setNode(node);
					return node;
				}
				else
					if (entry->info()->isFile())
						Application::open(m_container, entry->info());
		}

	return NULL;
}

Node *BaseNode::viewChild(const QString &fileName, QModelIndex &selected)
{
	Container::size_type index = m_items.indexOf(fileName);

	if (index == Container::InvalidIndex)
	{
		QString error;
		IFileInfo::Holder info;

		if (info = m_container->info(fileName, error))
			if (Node *node = createNode(info))
			{
				beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
				m_items.add(NodeItem::Holder(new NodeItem(info, node)));
				endInsertRows();

				return node;
			}
			else
			{
				if (info->isFile())
				{
					NodeItem *item;

					beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
					m_items.add(NodeItem::Holder(item = new NodeItem(info)));
					endInsertRows();

					selected = indexForFile(item, m_items.lastIndex());
				}

				return this;
			}
	}
	else
	{
		NodeItem *item = m_items[index].as<NodeItem>();

		if (item->node())
			return item->node();
		else
			if (Node *node = createNode(item->info()))
			{
				item->setNode(node);
				return node;
			}
			else
			{
				if (item->info()->isFile())
					selected = indexForFile(item, index);

				return this;
			}
	}

	return NULL;
}

void BaseNode::nodeRemoved(Node *node)
{
	Container::size_type index = m_items.indexOf(node);

	if (index != Container::InvalidIndex)
		m_items[index].as<NodeItem>()->setNode(NULL);
}

void BaseNode::createFile(const QModelIndex &index, INodeView *view)
{
	QString name;

	if (index.isValid())
	{
		NodeItem *item = m_items[m_proxy.mapToSource(index).row()].as<NodeItem>();

		if (!item->isRootItem())
			name = item->info()->fileName().as<QString>();
	}

	StringDialog dialog(tr("Enter name for new file"), tr("Name"), name, Application::mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	{

	}
}

void BaseNode::createDirectory(const QModelIndex &index, INodeView *view)
{
	QString name;

	if (index.isValid())
	{
		NodeItem *item = m_items[m_proxy.mapToSource(index).row()].as<NodeItem>();

		if (!item->isRootItem())
			name = item->info()->fileName().as<QString>();
	}

	StringDialog dialog(tr("Enter name for new directory"), tr("Name"), name, Application::mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	    if (m_container->contains(dialog.value()))
	    {
	        Container::size_type index = m_items.indexOf(dialog.value());

	        if (index == Container::InvalidIndex)
	        {
                QString error;
                NodeItem *item;
                IFileInfo::Holder info(m_container->info(dialog.value(), error));

                beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
                m_items.add(NodeItem::Holder(item = new NodeItem(info)));
                endInsertRows();

                view->select(indexForFile(item, m_items.size() - 1));
            }
	        else
                view->select(indexForFile(index));
	    }
	    else
	    {
            QString error;
            IFileContainer::Holder folder(m_container->create(dialog.value(), error));

            if (folder)
            {
                NodeItem *item;
                IFileInfo::Holder info(m_container->info(dialog.value(), error));

                beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
                m_items.add(NodeItem::Holder(item = new NodeItem(info)));
                endInsertRows();

                view->select(indexForFile(item, m_items.size() - 1));
            }
            else
                QMessageBox::critical(Application::mainWindow(), tr("Failed to create directory..."), error);
	    }
}

void BaseNode::rename(const QModelIndex &index, INodeView *view)
{
	QModelIndex idx = m_proxy.mapToSource(index);
	NodeItem *entry = static_cast<NodeItem *>(idx.internalPointer());

	if (!entry->isRootItem() && !static_cast<NodeItem *>(entry)->isLocked())
		renameFunctor(idx.row(), entry);
}

void BaseNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void BaseNode::remove(const QModelIndexList &list, INodeView *view)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForRemove(entries);
}

void BaseNode::cancel(const QModelIndexList &list, INodeView *view)
{
	CancelFunctor cancelFunctor(this, tr("Canceling..."));
	processLockedIndexList(list, cancelFunctor);
	updateFirstColumn(cancelFunctor.updateUnion());
}

void BaseNode::calculateSize(const QModelIndexList &list, INodeView *view)
{
	ProcessedList entries;
	processIndexList(list, entries);

	if (!entries.isEmpty())
		scanForSize(entries);
}

void BaseNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{
	AbsoluteFilePathList pathList(m_container.data());
	processIndexList(list, pathList);
	Application::instance()->clipboard()->setText(pathList.join(QChar(L'\r')));
}

void BaseNode::copy(const INodeView *source, INodeView *destination)
{
	ProcessedList entries;
	processIndexList(source->selectedIndexes(), entries);

	if (!entries.isEmpty())
		scanForCopy(entries, destination, false);
}

void BaseNode::move(const INodeView *source, INodeView *destination)
{
	ProcessedList entries;
	processIndexList(source->selectedIndexes(), entries);

	if (!entries.isEmpty())
		scanForCopy(entries, destination, true);
}

void BaseNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

void BaseNode::search(const QModelIndex &index, INodeView *view)
{
	SearchDialog dialog(Application::mainWindow());

	if (dialog.exec() == SearchDialog::Accepted)
	{
		IFileContainer::Holder container(m_container->open());
		FileNameFilter::Holder filter(new FileNameFilter(dialog.pattern()));
		switchTo(new SearchNode(container, filter, this), view);
	}
}

void BaseNode::cleanup(Snapshot &snapshot)
{
	Container::size_type index;

	for (Snapshot::iterator i = snapshot.begin(), end = snapshot.end(); i != end;)
		if (snapshot.isRemoved(i))
		{
			if ((index = m_items.indexOf((*i).first)) != Container::InvalidIndex)
				removeEntry(index);

			i = snapshot.erase(i);
		}
		else
			++i;
}

void BaseNode::processScanEventSnapshot(Snapshot &snapshot, EventFunctor &functor)
{
	Container::size_type index;

	for (Snapshot::iterator i = snapshot.begin(), end = snapshot.end(); i != end;)
		if ((index = m_items.indexOf((*i).first)) != Container::InvalidIndex)
		{
			functor(index, m_items[index], (*i).second);
			++i;
		}
		else
			i = snapshot.erase(i);
}

void BaseNode::processPerformEventSnapshot(Snapshot &snapshot, EventFunctor &functor)
{
	Container::size_type index;

	for (Snapshot::iterator i = snapshot.begin(), end = snapshot.end(); i != end;)
		if ((index = m_items.indexOf((*i).first)) != Container::InvalidIndex)
		{
			functor(index, m_items[index], (*i).second);
			++i;
		}
		else
			i = snapshot.erase(i);
}

Snapshot BaseNode::updateFilesList() const
{
	Snapshot::Files files(m_container.data());

	for (Container::size_type i = 0, size = m_items.size(); i < size; ++i)
        files.add(m_items[i].as<NodeItem>()->info()->fileName(), m_items[i]);

	return files;
}

void BaseNode::updateFilesEvent(Snapshot &updates)
{
	Union updateRange;
	Container::size_type index;

	for (Snapshot::iterator update = updates.begin(), end = updates.end(); update != end;)
		if (updates.isAdded(update) || updates.isUpdated(update))
		{
			if ((index = m_items.indexOf((*update).second->info()->fileName())) != Container::InvalidIndex)
			{
				m_items[index].as<NodeItem>()->update((*update).second->info());
				updateRange.add(index);

				update = updates.erase(update);
			}
			else
				++update;
		}
		else
		{
            if (updates.isRemoved(update) && (index = m_items.indexOf((*update).first)) != Container::InvalidIndex)
            {
                if (!m_items[index]->isLocked())
                    removeEntry(index);
            }

			update = updates.erase(update);
		}

	updateColumns(updateRange, columnCount(QModelIndex()) - 1);

	if (!updates.isEmpty())
	{
		beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + updates.size() - 1);
		for (Snapshot::iterator update = updates.begin(), end = updates.end(); update != end; update = updates.erase(update))
			m_items.add(NodeItem::Holder(new NodeItem((*update).second->info())));
		endInsertRows();
	}
}

void BaseNode::scanForSizeEvent(bool canceled, Snapshot &snapshot)
{
	if (canceled)
	{
		ScanForSizeEventFunctor_canceled functor;
		processScanEventSnapshot(snapshot, functor);
		updateBothColumns(functor.updateRange);
	}
	else
	{
		ScanForSizeEventFunctor functor;
		processScanEventSnapshot(snapshot, functor);
		updateBothColumns(functor.updateRange);
	}
}

bool BaseNode::scanForCopyEvent(Snapshot &snapshot, ICopyControl *control, bool move)
{
    ScanForCopyEventFunctor functor(move ? tr("Moving...") : tr("Copying..."));
    processScanEventSnapshot(snapshot, functor);

    if (control->start(snapshot, move))
    {
        updateBothColumns(functor.updateRange);
        return true;
    }
    else
        control->done(false);

    for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
        (*i).first.as<NodeItem>()->unlock();

    updateBothColumns(functor.updateRange);
    return false;
}

bool BaseNode::scanForRemoveEvent(bool canceled, Snapshot &snapshot)
{
	if (canceled)
	{
		ScanForRemoveEventFunctor_canceled functor;
		processScanEventSnapshot(snapshot, functor);
		updateBothColumns(functor.updateRange);
	}
	else
	{
		ScanForRemoveEventFunctor functor(tr("Removing..."));
		processScanEventSnapshot(snapshot, functor);

		if (QMessageBox::question(
				Application::mainWindow(),
				tr("Remove..."),
				tr("Would you like to remove").
					append(QString::fromLatin1("\n\t")).
					append(tr("directories:")).append(QString::fromLatin1("\n\t\t")).
					append(functor.folders.join(QString::fromLatin1("\n\t\t"))).
					append(QString::fromLatin1("\n\t")).
					append(tr("files:")).append(QString::fromLatin1("\n\t\t")).
					append(functor.files.join(QString::fromLatin1("\n\t\t"))).
					append(QString::fromLatin1("\n")).
					append(tr("it will free ").append(Tools::humanReadableSize(snapshot.totalSize()))),
				QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			updateSecondColumn(functor.updateRange);
			return true;
		}
		else
			for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
				(*i).first.as<NodeItem>()->unlock();

		updateBothColumns(functor.updateRange);
	}

	return false;
}

bool BaseNode::performCopyEvent(bool canceled, Snapshot &snapshot, ICopyControl *control, bool move)
{
    Container::size_type index;

    if (canceled)
	{
        for (Snapshot::iterator update = snapshot.begin(), end = snapshot.end(); update != end; update = snapshot.erase(update))
            if ((index = m_items.indexOf((*update).first)) != Container::InvalidIndex && m_items[index].as<NodeItem>()->isTmpItem())
                if (m_items[index].as<TmpNodeItem>()->originalItem())
                    m_items[index] = m_items[index].as<TmpNodeItem>()->originalItem();
                else
                    removeEntry(index);

        control->canceled();
	}
	else
        if (move)
        {
            for (Snapshot::iterator update = snapshot.begin(), end = snapshot.end(); update != end; ++update)
                if ((index = m_items.indexOf((*update).first)) != Container::InvalidIndex && m_items[index].as<NodeItem>()->isTmpItem())
                    if (m_items[index].as<TmpNodeItem>()->originalItem())
                        m_items[index] = m_items[index].as<TmpNodeItem>()->originalItem();

            control->done(false);

            PerformCopyEventFunctor functor(tr("Removing..."));
            processPerformEventSnapshot(snapshot, functor);
            updateSecondColumn(functor.updateRange);

            return true;
        }
        else
        {
            for (Snapshot::iterator update = snapshot.begin(), end = snapshot.end(); update != end; ++update)
                if ((index = m_items.indexOf((*update).first)) != Container::InvalidIndex && m_items[index].as<NodeItem>()->isTmpItem())
                    if (m_items[index].as<TmpNodeItem>()->originalItem())
                        m_items[index] = m_items[index].as<TmpNodeItem>()->originalItem();
                    else
                        removeEntry(index);

            control->done(false);
        }

    PerformCopyEventFunctor_canceled functor;
    processPerformEventSnapshot(snapshot, functor);
    updateBothColumns(functor.updateRange);

	return false;
}

void BaseNode::performRemoveEvent(bool canceled, Snapshot &snapshot)
{
	Union updateRange;
	PerformRemoveEventFunctor functor(this, &BaseNode::removeEntry);

	processPerformEventSnapshot(snapshot, functor);

	for (QStringList::size_type i = 0, size = functor.list.size(); i < size; ++i)
		updateRange.add(m_items.indexOf(functor.list.at(i)));

	updateBothColumns(updateRange);
}

void BaseNode::updateProgressEvent(const NodeItem::Holder &item, quint64 progress, quint64 timeElapsed)
{
	item.as<NodeItem>()->updateProgress(progress, timeElapsed);
	updateSecondColumn(m_items.indexOf(item.as<NodeItem>()->info()->fileName()), item.as<NodeItem>());
}

void BaseNode::completedProgressEvent(const NodeItem::Holder &item, quint64 timeElapsed)
{
	item.as<NodeItem>()->updateProgress(item.as<NodeItem>()->total(), timeElapsed);
	updateSecondColumn(m_items.indexOf(item.as<NodeItem>()->info()->fileName()), item.as<NodeItem>());
}

void BaseNode::performActionEvent(const AsyncAction::FilesList &files, const QString &error)
{
	Union update;
	NodeItem::Holder item;

	for (Action::FilesList::size_type i = 0, size = files.size(); i < size; ++i)
	{
		item = files.at(i).first;

		item->unlock();
		update.add(m_items.indexOf(item.data()));
	}

	if (!error.isEmpty())
		QMessageBox::critical(Application::mainWindow(), tr("Error..."), error);

	updateFirstColumn(update);
}

Node *BaseNode::createNode(const IFileInfo *file) const
{
	return Application::rootNode()->open(m_container.data(), file, const_cast<BaseNode *>(this));
}

BaseNode::Container::Container()
{}

BaseNode::Container::size_type BaseNode::Container::size() const
{
    return m_container.size();
}

BaseNode::Container::Item *BaseNode::Container::at(size_type index) const
{
    return m_container.at(index).data();
}

BaseNode::Container::size_type BaseNode::Container::indexOf(Item *item) const
{
    for (List::size_type i = 0, size = m_container.size(); i < size; ++i)
        if (static_cast<Item *>(m_container.at(i).data()) == item)
            return i;

    return InvalidIndex;
}

void BaseNode::updateFiles()
{
	if (isVisible())
	{
		::Tools::Memory::ScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::UpdateFiles, this, updateFilesList()));
		setUpdating(true);
		handleTask(task.take());
	}
}

void BaseNode::scanForSize(const Snapshot &snapshot)
{
	::Tools::Memory::ScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::ScanFilesForSize, this, snapshot));
	addTask(task.take(), snapshot);
}

void BaseNode::scanForCopy(const Snapshot &snapshot, ICopyControl::Holder &destination, bool move)
{
	::Tools::Memory::ScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::ScanFilesForCopy, this, destination, snapshot, move));
	addTask(task.data(), task.const_data()->destination().data(), snapshot);
	task.take();
}

void BaseNode::scanForRemove(const Snapshot &snapshot)
{
	::Tools::Memory::ScopedPointer<ScanFilesTask> task(new ScanFilesTask(FilesBaseTask::Event::ScanFilesForRemove, this, snapshot));
	addTask(task.take(), snapshot);
}

void BaseNode::performCopy(BaseTask *oldTask, const Snapshot &snapshot, ICopyControl::Holder &destination, bool move)
{
	if (destination->isDefault() && move)
	{
		::Tools::Memory::ScopedPointer<PerformMoveTask> task(new PerformMoveTask(this, destination, snapshot));
		resetTask(task.data(), oldTask);
		task.take();
	}
	else
	{
		::Tools::Memory::ScopedPointer<PerformCopyTask> task(new PerformCopyTask(this, destination, snapshot, move));
		resetTask(task.data(), oldTask);
		task.take();
	}
}

void BaseNode::performRemove(BaseTask *oldTask, const Snapshot &snapshot)
{
	::Tools::Memory::ScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, snapshot));
	resetTask(task.take(), oldTask);
}

void BaseNode::performRemove(BaseTask *oldTask, const ICopyControl *destination, const Snapshot &snapshot)
{
	::Tools::Memory::ScopedPointer<PerformRemoveTask> task(new PerformRemoveTask(this, snapshot));
	resetTask(task.take(), oldTask, destination);
}

void BaseNode::updateFiles(const BaseTask::Event *e)
{
	typedef ScanFilesTask::UpdatesEvent * NotConstEvent;
	NotConstEvent event = static_cast<NotConstEvent>(const_cast<BaseTask::Event *>(e));

	if (!event->canceled)
		updateFilesEvent(event->snapshot);

	if (event->isLastEvent)
	{
		setUpdating(false);
		taskHandled(event->task);
	}
}

void BaseNode::scanForSize(const BaseTask::Event *e)
{
	typedef ScanFilesTask::Event * NotConstEvent;
	typedef const ScanFilesTask::Event * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	scanForSizeEvent(event->canceled, event->snapshot);
	removeAllTaskLinks(event->task);
}

void BaseNode::scanForCopy(const BaseTask::Event *e)
{
	typedef ScanFilesTask::CopyEvent * NotConstEvent;
	typedef const ScanFilesTask::CopyEvent * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

    if (event->canceled)
    {
        ScanForCopyEventFunctor_canceled functor;
        processScanEventSnapshot(event->snapshot, functor);

        const_cast<NotConstEvent>(event)->destination->canceled();
        updateBothColumns(functor.updateRange);
    }
    else
    {
        cleanup(event->snapshot);

        if (scanForCopyEvent(event->snapshot, event->destination.data(), event->move))
            performCopy(event->task, event->snapshot, const_cast<NotConstEvent>(event)->destination, event->move);
        else
            removeAllTaskLinks(event->task, event->destination.data());
    }
}

void BaseNode::scanForRemove(const BaseTask::Event *e)
{
	typedef ScanFilesTask::Event * NotConstEvent;
	typedef const ScanFilesTask::Event * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	if (scanForRemoveEvent(event->canceled, event->snapshot))
		performRemove(event->task, event->snapshot);
	else
		removeAllTaskLinks(event->task);
}

void BaseNode::scanClipboardFiles(const BaseTask::Event *e)
{
    typedef ScanClipboardFilesTask::CopyEvent * NotConstEvent;
    NotConstEvent event = static_cast<NotConstEvent>(const_cast<BaseTask::Event *>(e));

    if (event->canceled)
    {
        Container::size_type index;

        for (Snapshot::iterator update = event->snapshot.begin(), end = event->snapshot.end(); update != end; update = event->snapshot.erase(update))
            if ((index = m_items.indexOf((*update).first)) != Container::InvalidIndex)
                removeEntry(index);

        removeAllTaskLinks(event->task, event->destination.data());
    }
    else
    {
        Container::size_type index;
        Container::size_type index2;

        for (Snapshot::iterator update = event->snapshot.begin(), end = event->snapshot.end(); update != end;)
            if (event->snapshot.isRemoved(update))
            {
                if ((index = m_items.indexOf((*update).first)) != Container::InvalidIndex)
                    removeEntry(index);

                update = event->snapshot.erase(update);
            }
            else
                if ((index = m_items.indexOf((*update).first)) != Container::InvalidIndex)
                {
                    index2 = m_items.indexOf((*update).second->info()->fileName());

                    if (index2 == Container::InvalidIndex)
                        ++update;
                    else
                        if (m_items[index2]->isLocked())
                            if (QMessageBox::question(Application::mainWindow(),
                                    tr("File already exists"),
                                    tr("File \"%1\" is locked because of \"%2\" and therefore can not be replaced.").
                                        arg((*update).second->info()->fileName()).
                                        arg(m_items[index2]->lockReason()).
                                        append(QChar(L'\n')).
                                        append(tr("Skip it?")),
                                    QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                            {
                                removeEntry(index);
                                update = event->snapshot.erase(update);
                            }
                            else
                            {
                                for (update = event->snapshot.begin(); update != end; update = event->snapshot.erase(update))
                                    if ((index = m_items.indexOf((*update).first)) != Container::InvalidIndex)
                                        removeEntry(index);

                                removeAllTaskLinks(event->task, event->destination.data());
                                return;
                            }
                        else
                        {
                            (*update).first.as<TmpNodeItem>()->originalItem() = m_items[index2];
                            m_items[index2] = (*update).first;
                            removeEntry(index);
                            ++update;
                        }
                }
                else
                    update = event->snapshot.erase(update);

        if (scanForCopyEvent(event->snapshot, event->destination.data(), event->move))
            performCopy(event->task, event->snapshot, const_cast<NotConstEvent>(event)->destination, event->move);
        else
        {
            for (Snapshot::iterator update = event->snapshot.begin(), end = event->snapshot.end(); update != end; update = event->snapshot.erase(update))
                if ((index = m_items.indexOf((*update).first)) != Container::InvalidIndex)
                    if (m_items[index].as<TmpNodeItem>()->originalItem())
                        m_items[index] = m_items[index].as<TmpNodeItem>()->originalItem();
                    else
                        removeEntry(index);

            removeAllTaskLinks(event->task, event->destination.data());
        }
    }
}

void BaseNode::performCopy(const BaseTask::Event *e)
{
	typedef PerformCopyTask::ExtendedEvent * NotConstEvent;
	typedef const PerformCopyTask::ExtendedEvent * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	if (performCopyEvent(event->canceled, event->snapshot, event->destination.data(), event->move))
		performRemove(event->task, event->destination.data(), event->snapshot);
	else
		removeAllTaskLinks(event->task, event->destination.data());
}

void BaseNode::performRemove(const BaseTask::Event *e)
{
	typedef PerformRemoveTask::Event * NotConstEvent;
	typedef const PerformRemoveTask::Event * Event;
	NotConstEvent event = const_cast<NotConstEvent>(static_cast<Event>(e));

	performRemoveEvent(event->canceled, event->snapshot);
	removeAllTaskLinks(event->task);
}

void BaseNode::copyThroughClipboard(INodeView *view, const QModelIndexList &list, bool move)
{
#if PLATFORM_DE(KDE)
    static const char suffix[] = "\r\n";
#else
    static const char suffix[] = "\n";
#endif

    ::Tools::Memory::ScopedPointer<ClipboardMimeData> data(new ClipboardMimeData(move));

    CopyFilesThroughClipboard functor(m_container,
            QByteArray(m_container->schema()).append("://"),
            QByteArray::fromRawData(suffix, qstrlen(suffix)));

    processIndexList(list, functor);

    data->setData(functor.data());
    Application::clipboard()->setMimeData(data.take(), QClipboard::Clipboard);
}

void BaseNode::pasteThroughClipboard(INodeView *view)
{
    QList<QByteArray> files = Application::clipboard()->mimeData()->data(uriListMimetype).split('\n');

    if (!files.isEmpty())
    {
        ICopyControl::Holder dest(m_container->createControl(view));

        if (dest)
        {
            Snapshot::Files snapshot(m_container.data());
            IFileInfo::Holder info;
            NodeItem::Holder item;
            bool move = false;
#if PLATFORM_DE(KDE)
            move = !Application::clipboard()->mimeData()->data(kdeCutMimetype).isEmpty();
#endif

            beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + files.size() - 1);
            for (QList<QByteArray>::size_type i = 0, size = files.size(); i < size; ++i)
            {
#if PLATFORM_DE(KDE)
                files[i] = QByteArray::fromPercentEncoding(files.at(i)).trimmed();
#endif
                info = new InvalidInfo(files.at(i));
                item = new TmpNodeItem(info);
                item->lock(move ? tr("Scanning for move...") : tr("Scanning for copy..."));
                snapshot.add(item.as<NodeItem>()->info()->fileName(), item);
                m_items.add(item);
            }
            endInsertRows();

            ::Tools::Memory::ScopedPointer<ScanClipboardFilesTask> task(new ScanClipboardFilesTask(this, dest, snapshot, move));
            addTask(task.data(), task.const_data()->destination().data(), snapshot);
            task.take();
        }
    }
}

void BaseNode::ProcessedList::call(Container::size_type index, NodeItem *item)
{
    push_back(ProcessedValue(index, item));
}

void BaseNode::AbsoluteFilePathList::call(Container::size_type index, NodeItem *item)
{
    push_back(m_container->location(item->info()));
}

void BaseNode::CopyFilesThroughClipboard::call(Container::size_type index, NodeItem *item)
{
    m_data.append(m_prefix).append(m_container->location(item->info()).as<QByteArray>()).append(m_suffix);
}

void BaseNode::CancelFunctor::call(Container::size_type index, NodeItem *item)
{
    m_items = m_node->cancelTaskAndTakeItems(NodeItem::Holder(item));

    if (!m_items.isEmpty())
    {
        m_union.add(index);

        for (TasksMap::List::size_type i = 0, size = m_items.size(); i < size; ++i)
            m_items.at(i).as<TasksNodeItem>()->cancel(m_reason);
    }
}

void BaseNode::renameFunctor(Container::size_type index, NodeItem *item)
{
	StringDialog dialog(
			item->info()->isDir() ?
					tr("Enter new name for directory \"%1\"").arg(item->info()->fileName()) :
					tr("Enter new name for file \"%1\"").arg(item->info()->fileName()),
			tr("Name"),
			item->info()->fileName(),
			Application::mainWindow());

	if (dialog.exec() == QDialog::Accepted)
	{
		QString error;

		if (m_container->rename(item->info(), dialog.value(), error))
		{
			IFileInfo::Holder info(m_container->info(dialog.value(), error));

			if (info)
			{
				item->update(info);
				updateColumns(index, item, columnsCount());
			}
			else
				QMessageBox::critical(Application::mainWindow(),
							item->info()->isDir() ?
								tr("Failed to rename directory \"%1\"").arg(item->info()->fileName()) :
								tr("Failed to rename file \"%1\"").arg(item->info()->fileName()),
							error);
		}
		else
			QMessageBox::critical(Application::mainWindow(),
						item->info()->isDir() ?
							tr("Failed to rename directory \"%1\"").arg(item->info()->fileName()) :
							tr("Failed to rename file \"%1\"").arg(item->info()->fileName()),
						error);
	}
}

void BaseNode::processIndexList(const QModelIndexList &list, Functor &functor)
{
	QModelIndex index;
	NodeItem *entry;
	QSet<NodeItem *> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<NodeItem *>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && !static_cast<NodeItem *>(entry)->isLocked())
				functor(index.row(), entry);
		}
}

void BaseNode::processLockedIndexList(const QModelIndexList &list, Functor &functor)
{
	QModelIndex index;
	ProcessedList res;
	NodeItem *entry;
	QSet<NodeItem *> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<NodeItem *>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && static_cast<NodeItem *>(entry)->isLocked())
				functor(index.row(), entry);
		}
}

void BaseNode::scanForRemove(const ProcessedList &entries)
{
	Union updateRange;
	NodeItem *entry;
	Container::size_type index;
	Snapshot::Files files(m_container.data());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
	{
		index = entries.at(i).first;

		if ((entry = entries.at(i).second)->info()->isDir())
			entry->lock(tr("Scanning folder for remove..."));
		else
			entry->lock(tr("Removing..."));

		updateRange.add(index);
		files.add(entry->info()->fileName(), NodeItem::Holder(entry));
	}

	updateFirstColumn(updateRange);
	scanForRemove(files);
}

void BaseNode::scanForSize(const ProcessedList &entries)
{
	Union updateRange;
	NodeItem *entry;
	Container::size_type index;
	Snapshot::Files files(m_container.data());

	for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		if ((entry = static_cast<NodeItem*>(entries.at(i).second))->info()->isDir())
		{
			index = entries.at(i).first;
			entry->lock(tr("Scanning folder for size..."));
			updateRange.add(index);
			files.add(entry->info()->fileName(), NodeItem::Holder(entry));
		}

	updateFirstColumn(updateRange);
	scanForSize(files);
}

void BaseNode::scanForCopy(const ProcessedList &entries, INodeView *destination, bool move)
{
	ICopyControl::Holder control(destination->node()->createControl(destination));

	if (control)
	{
		Union updateRange;
		NodeItem *entry;
		Container::size_type index;
		Snapshot::Files files(m_container.data());
		QString fileLockReason = move ? tr("Moving...") : tr("Copying...");
		QString folderLockReason = move ? tr("Scanning folder for move...") : tr("Scanning folder for copy...");

		for (ProcessedList::size_type i = 0, size = entries.size(); i < size; ++i)
		{
			index = entries.at(i).first;

			if ((entry = static_cast<NodeItem*>(entries.at(i).second))->info()->isDir())
				entry->lock(folderLockReason);
			else
				entry->lock(fileLockReason);

			updateRange.add(index);
			files.add(entry->info()->fileName(), NodeItem::Holder(entry));
		}

		updateFirstColumn(updateRange);
		scanForCopy(files, control, move);
	}
}

QModelIndex BaseNode::index(int column, NodeItem *item) const
{
	Container::size_type index = m_items.indexOf(item);

	if (index != Container::InvalidIndex)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

QModelIndex BaseNode::indexForFile(NodeItem *item) const
{
	Q_ASSERT(m_items.indexOf(item) != Container::InvalidIndex);
	Container::size_type index = m_items.indexOf(item);
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

QModelIndex BaseNode::indexForFile(Container::size_type index) const
{
    return m_proxy.mapFromSource(createIndex(index, 0, m_items[index].data()));
}

QModelIndex BaseNode::indexForFile(NodeItem *item, Container::size_type index) const
{
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

void BaseNode::updateFirstColumn(NodeItem *entry)
{
	updateFirstColumn(m_items.indexOf(entry), entry);
}

void BaseNode::updateFirstColumn(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), 0, m_items[range.at(i).bottom()].data()));
}

void BaseNode::updateFirstColumn(Container::size_type index, NodeItem *entry)
{
	QModelIndex idx = createIndex(index, 0, entry);
	emit dataChanged(idx, idx);
}

void BaseNode::updateSecondColumn(NodeItem *entry)
{
	updateSecondColumn(m_items.indexOf(entry), entry);
}

void BaseNode::updateSecondColumn(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 1, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), 1, m_items[range.at(i).bottom()].data()));
}

void BaseNode::updateSecondColumn(Container::size_type index, NodeItem *entry)
{
	QModelIndex idx = createIndex(index, 1, entry);
	emit dataChanged(idx, idx);
}

void BaseNode::updateBothColumns(NodeItem *entry)
{
	updateBothColumns(m_items.indexOf(entry), entry);
}

void BaseNode::updateBothColumns(const Union &range)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), 1, m_items[range.at(i).bottom()].data()));
}

void BaseNode::updateBothColumns(Container::size_type index, NodeItem *entry)
{
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, 1, entry));
}

void BaseNode::updateColumns(const Union &range, int lastColumn)
{
	for (Union::List::size_type i = 0, size = range.size(); i < size; ++i)
		emit dataChanged(createIndex(range.at(i).top(), 0, m_items[range.at(i).top()].data()),
						 createIndex(range.at(i).bottom(), lastColumn, m_items[range.at(i).bottom()].data()));
}

void BaseNode::updateColumns(Container::size_type index, NodeItem *entry, int lastColumn)
{
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, lastColumn, entry));
}

void BaseNode::removeEntry(Container::size_type index)
{
	beginRemoveRows(QModelIndex(), index, index);
	m_items.remove(index);
	endRemoveRows();
}

void BaseNode::removeEntry(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	m_items.remove(index.row());
	endRemoveRows();
}

DEFAULT_PLUGIN_NS_END
