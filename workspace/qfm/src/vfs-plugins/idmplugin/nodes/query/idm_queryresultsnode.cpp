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
#include "idm_queryresultsnode.h"
#include "functors/idm_queryresultsfunctor.h"
#include "control/idm_queryresultscopycontrol.h"
#include "events/idm_queryresultsmodelevents.h"
#include "tasks/scan/idm_queryresultsnode_scantask.h"
#include "tasks/perform/idm_queryresultsnode_performremovetask.h"
#include "items/idm_queryresultvalueitem.h"
#include "items/idm_queryresultpropertyitem.h"
#include "items/idm_queryresultrootpathvalueitem.h"
#include "items/idm_queryresultpathpropertyitem.h"
#include "items/idm_queryresultcompositerootitem.h"
#include "../folder/idm_foldernode.h"
#include "../../gui/value/list/selectable/idm_selectablevaluelistdialog.h"

#include "../../../default/actions/default_globalactions.h"

#include <application.h>
#include <vfs/tools/vfs_commontools.h>
#include <vfs/contextmenu/vfs_contextmenu.h>

#include <vfs/tasks/vfs_performactiontask.h>
#include <vfs/actions/synchronous/vfs_syncaction.h>
#include <vfs/actions/asynchronous/vfs_asyncaction.h>

#include <tools/containers/union.h>
#include <tools/widgets/stringdialog/stringdialog.h>

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

QueryResultsNode::QueryResultsNode(const IdmContainer &container, const EntityValueReader &reader, Node *parent) :
    TasksNode(m_itemsContainer, parent),
    m_items(m_itemsContainer.m_container),
    m_delegate(container),
    m_container(container),
    m_reader(reader),
    m_label(tr("Found \"%1\" entities...").arg(toUnicode(reader.entity().name())))
{
    m_shortcuts[Qt::NoModifier + Qt::Key_F2]     = RenameShortcut;
    m_shortcuts[Qt::NoModifier + Qt::Key_Insert] = CreateShortcut;
    m_shortcuts[Qt::SHIFT      + Qt::Key_Delete] = RemoveShortcut;
    m_shortcuts[Qt::NoModifier + Qt::Key_Delete] = RemoveShortcut;
    m_shortcuts[Qt::CTRL       + Qt::Key_R]      = RefreshShortcut;
}

bool QueryResultsNode::event(QEvent *e)
{
    switch (static_cast<ModelEvent::Type>(e->type()))
    {
        case ModelEvent::UpdateFiles:
        {
            e->accept();
            scanUpdates(static_cast<BaseTask::Event*>(e));
            return true;
        }
        case ModelEvent::ScanFilesForRemove:
        {
            e->accept();
            scanForRemove(static_cast<BaseTask::Event*>(e));
            return true;
        }
        case ModelEvent::RemoveFiles:
        {
            e->accept();
            performRemove(static_cast<BaseTask::Event*>(e));
            return true;
        }
        default:
            break;
    }

    return TasksNode::event(e);
}

int QueryResultsNode::columnCount(const QModelIndex &parent) const
{
    return 1;
}

void QueryResultsNode::fetchMore(const QModelIndex &parent)
{
    EntityValue item;
    ItemsContainer::List list;
    Snapshot::Files files(m_container.container());

    list.reserve(PrefetchLimit);

    if (m_reader.entity().type() == Entity::Composite)
        for (qint32 actualLimit = 0; actualLimit < PrefetchLimit; ++actualLimit)
            if ((item = m_reader.next()).isValid())
                list.push_back(Item::Holder(new QueryResultCompositeRootItem(files, m_container, item)));
            else
                break;
    else
        for (qint32 actualLimit = 0; actualLimit < PrefetchLimit; ++actualLimit)
            if ((item = m_reader.next()).isValid())
                list.push_back(Item::Holder(new QueryResultRootItem(item)));
            else
                break;

    if (!list.isEmpty())
    {
        if (!files.isEmpty())
            handleTask(new ScanFilesTask(ModelEvent::UpdateFiles, this, files));

        beginInsertRows(parent, m_items.size(), m_items.size() + list.size() - 1);
        m_items.append(list);
        endInsertRows();
    }
}

bool QueryResultsNode::canFetchMore(const QModelIndex &parent) const
{
    return !parent.isValid() && !m_reader.eof();
}

Qt::ItemFlags QueryResultsNode::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant QueryResultsNode::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0 && orientation == Qt::Horizontal)
        switch (role)
        {
            case Qt::DisplayRole:
                return m_label;

            case Qt::TextAlignmentRole:
                return Qt::AlignCenter;

            default:
                break;
        }

    return QVariant();
}

ICopyControl *QueryResultsNode::createControl(INodeView *view) const
{
    QModelIndex index = view->currentIndex();

    if (static_cast<QueryResultItem *>(index.internalPointer())->isProperty())
    {
        QueryResultPropertyItem *item = static_cast<QueryResultPropertyItem *>(index.internalPointer());

//        if (item->property().entity.type() == Database::Path)
//            if (item->size() == 0)
//            {
//                ICopyControl::Holder control(m_container.container()->createControl(view));
//
//                if (control)
//                    return new IdmQueryResultsCopyControl(
//                            control,
//                            m_container,
//                            static_cast<QueryResultRootItem *>(item->parent())->value(),
//                            item->property(),
//                            const_cast<QueryResultsNode *>(this),
//                            index);
//            }
//            else
//            {
//                QString destination;
//                Tools::DestinationFromPathList tree;
//
//                for (QueryResultPropertyItem::size_type i = 0, size = item->size(); i < size; ++i)
//                    if (static_cast<QueryResultPathItem *>(item->at(i))->info()->isFile())
//                        tree.add(static_cast<QueryResultPathItem *>(item->at(i))->location(), 1);
//                    else
//                        tree.add(static_cast<QueryResultPathItem *>(item->at(i))->location());
//
//                if (!(destination = tree.choose(tr("Choose a directory"), Application::mainWindow())).isEmpty())
//                {
//                    QString error;
//                    IFileContainer::Holder container(m_container.container()->create(destination, error));
//
//                    if (container)
//                    {
//                        ICopyControl::Holder control(container->createControl(view));
//
//                        return new IdmQueryResultsCopyControl(
//                                control,
//                                m_container,
//                                static_cast<QueryResultRootItem *>(item->parent())->value(),
//                                item->property(),
//                                const_cast<QueryResultsNode *>(this),
//                                index);
//                    }
//                    else
//                        QMessageBox::critical(Application::mainWindow(), tr("Error"), error);
//                }
//            }
    }

    return NULL;
}

void QueryResultsNode::contextMenu(const QModelIndexList &list, INodeView *view)
{
    typedef QSet<QueryResultItem::Holder>              ItemsSet;
    typedef QList<QueryResultItem::Holder>             ItemsList;
    typedef QMap<QueryResultItem::Holder, QModelIndex> ItemsIndexMap;

    ItemsSet set;
    ItemsList items;
    QueryResultItem::Holder item;
    ItemsIndexMap itemsIndex;
    ContextMenu contextMenu(m_container.container());

    for (ItemsList::size_type i = 0, size = list.size(); i < size; ++i)
        if ((item = static_cast<QueryResultItem *>(list.at(i).internalPointer())).as<QueryResultItem>()->isPath() && !set.contains(item))
        {
            set.insert(item);
            itemsIndex[item] = list.at(i);
            contextMenu.add(item, item.as<QueryResultPathItem>()->info());
        }

    items = set.toList();

    contextMenu.add(Default::GlobalActions::instance()->copyAction(), ContextMenu::GeneralSection);
    contextMenu.add(Default::GlobalActions::instance()->cutAction(), ContextMenu::GeneralSection);

    if (items.size() == 1 && items.at(0).as<QueryResultPathItem>()->info()->isDir())
        contextMenu.add(Default::GlobalActions::instance()->pasteIntoFolderAction(), ContextMenu::GeneralSection);

    contextMenu.add(Default::GlobalActions::instance()->propertiesAction(), ContextMenu::PropertiesSection);

    if (const Action *action = contextMenu.exec())
    {
        Action::FilesList files = contextMenu.files(action);

        if (action->isAsynchronous())
        {
            ::Tools::Memory::ScopedPointer<PerformActionTask> task;

            if (task = static_cast<const AsyncAction *>(action)->process(this, m_container.container(), files))
            {
                Union update;
                Snapshot::Files list(m_container.container());

                for (Action::FilesList::size_type i = 0, size = files.size(); i < size; ++i)
                {
                    item = files.at(i).first;

//                    item->lock(static_cast<const AsyncAction *>(action)->lockReason());
//                    update.add(itemsIndex.value(item));
                    list.add(item.as<QueryResultPathItem>()->info()->fileName(), item);
                }

//                addTask(task.take(), list);
//                updateFirstColumn(update);
            }
        }
        else
            static_cast<const SyncAction *>(action)->process(m_container.container(), files);
    }
}

void QueryResultsNode::cancel(const QModelIndexList &list, INodeView *view)
{

}

void QueryResultsNode::refresh()
{
//    QueryResultItem *item;
//    Snapshot::Files files(m_container.container());
//
//    for (ItemsContainer::size_type i = 0, size = m_items.size(); i < size; ++i)
//        for (QueryResultItem::size_type q = 0, size = (item = static_cast<QueryResultItem *>(m_items.at(i))); q < size; ++q)
//            if (static_cast<QueryResultPropertyItem *>(item->at(q))->property().entity.type() == Database::Path)
//            {
//
//            }
}

QString QueryResultsNode::title() const
{
    QString res = m_container.container()->location();
    return res.mid(res.lastIndexOf(QChar(L'/')));
}

QString QueryResultsNode::location() const
{
    return m_container.container()->location();
}

QString QueryResultsNode::fileName(const QModelIndex &index) const
{
    return QString();
}

bool QueryResultsNode::shortcut(INodeView *view, QKeyEvent *event)
{
    switch (m_shortcuts.value(event->modifiers() + event->key(), NoShortcut))
    {
        case CreateShortcut:
            create(view->currentIndex(), view);
            return true;

        case RenameShortcut:
            rename(view->currentIndex(), view);
            return true;

        case RemoveShortcut:
            remove(view->selectedIndexes(), view);
            return true;

        case RefreshShortcut:
            refresh(view);
            return true;

        default:
            return false;
    }
}

QueryResultsNode::Sorting QueryResultsNode::sorting() const
{
    return Sorting(0, Qt::AscendingOrder);
}

QueryResultsNode::Geometry QueryResultsNode::geometry() const
{
    return Geometry() << 100;
}

QAbstractItemModel *QueryResultsNode::model() const
{
    return const_cast<QueryResultsNode *>(this);
}

QAbstractItemDelegate *QueryResultsNode::delegate() const
{
    return const_cast<QueryResultsDelegate *>(&m_delegate);
}

const INodeView::MenuActionList &QueryResultsNode::actions() const
{
    return m_actions;
}

void QueryResultsNode::menuAction(INodeView *view, QAction *action)
{

}

QModelIndex QueryResultsNode::rootIndex() const
{
    return QModelIndex();
}

QModelIndex QueryResultsNode::childIndex(const QString &fileName)
{
    return QModelIndex();
}

Node *QueryResultsNode::viewChild(const QModelIndex &idx, QModelIndex &selected)
{
    QueryResultItem *item = static_cast<QueryResultItem *>(idx.internalPointer());

    if (item == NULL)
        return parentNode();
    else
        if (!item->isLocked() && item->isPath())
            if (Node *node = static_cast<QueryResultPathItem *>(item)->node())
                return node;
            else
                if (static_cast<QueryResultPathItem *>(item)->info()->isDir())
                {
                    QString error;
                    IFileContainer::Holder folder(static_cast<QueryResultPathItem *>(item)->info()->open(error));

                    if (folder)
                    {
                        node = new FolderNode(folder, m_container, this);
                        static_cast<QueryResultPathItem *>(item)->setNode(node);
                        return node;
                    }
                    else
                        QMessageBox::critical(Application::mainWindow(), tr("Error"), error);
                }
                else
                    if (static_cast<QueryResultPathItem *>(item)->info()->isFile())
                        static_cast<QueryResultPathItem *>(item)->open();

    return NULL;
}

Node *QueryResultsNode::viewChild(const QString &fileName, QModelIndex &selected)
{
    return NULL;
}

void QueryResultsNode::updateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed)
{

}

void QueryResultsNode::completedProgressEvent(const Item::Holder &item, quint64 timeElapsed)
{

}

void QueryResultsNode::performActionEvent(const AsyncAction::FilesList &files, const QString &error)
{

}

void QueryResultsNode::add(const QModelIndex &index, const EntityValue::List &values)
{
    QueryResultPathPropertyItem *item = static_cast<QueryResultPathPropertyItem *>(index.internalPointer());

    beginInsertRows(index, item->size(), item->size() + values.size() - 1);
    item->add(m_container.container(), values);
    endInsertRows();
}

void QueryResultsNode::remove(const QModelIndex &index, const EntityValue::List &values)
{

}

void QueryResultsNode::refresh(const QModelIndex &index)
{
    Snapshot::Files files(m_container.container());
    QueryResultItem *item = static_cast<QueryResultItem *>(index.internalPointer());

    for (QueryResultItem::size_type i = 0, size = item->size(); i < size; ++i)
        files.add(static_cast<QueryResultPathItem *>(item->at(i))->location(), Item::Holder(static_cast<QueryResultItem *>(item->at(i))));

    if (!files.isEmpty())
        handleTask(new ScanFilesTask(ModelEvent::UpdateFiles, this, files));
}

void QueryResultsNode::create(const QModelIndex &index, INodeView *view)
{
    if (static_cast<QueryResultItem *>(index.internalPointer())->isProperty())
    {
        QueryResultPropertyItem *item = static_cast<QueryResultPropertyItem *>(index.internalPointer());

        if (m_container.transaction())
        {
            EntityValueReader reader(m_container.entityValues(item->property().entity));
            SelectableValueListDialog dialog(m_container, reader, Application::mainWindow());

            if (dialog.exec() == SelectableValueListDialog::Accepted)
            {
                EntityValue value = dialog.takeValue();

                if (m_container.addValue(static_cast<QueryResultRootItem *>(item->parent())->value(), value))
                    if (m_container.commit())
//                        if (item->property().entity.type() == Database::Path)
//                        {
//                            beginInsertRows(index, item->size(), item->size());
//                            static_cast<QueryResultPathPropertyItem *>(item)->add(m_container.container(), value);
//                            endInsertRows();
//                        }
//                        else
                        {
                            beginInsertRows(index, item->size(), item->size());
                            item->add(value);
                            endInsertRows();
                        }
                    else
                    {
                        QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
                        m_container.rollback();
                    }
                else
                {
                    QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
                    m_container.rollback();
                }
            }
            else
                m_container.rollback();
        }
        else
            QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
    }
}

void QueryResultsNode::rename(const QModelIndex &index, INodeView *view)
{
    view->edit(index);
}

void QueryResultsNode::remove(const QModelIndexList &list, INodeView *view)
{
    if (m_container.transaction())
    {
        QModelIndex index;
        Snapshot::Files files(m_container.container());
        QueryResultValueItem *valueItem;
        QueryResultRootPathValueItem *pathItem;
        QueryResultPropertyItem *property;
        QueryResultPropertyItem::size_type idx;

        for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
            if (static_cast<QueryResultItem *>((index = list.at(i)).internalPointer())->isRootPathValue())
            {
                pathItem = static_cast<QueryResultRootPathValueItem *>(index.internalPointer());

                if (!pathItem->isLocked())
                    files.add(pathItem->location(), Item::Holder(pathItem));
            }
            else
                if (static_cast<QueryResultItem *>(index.internalPointer())->isValue())
                {
                    valueItem = static_cast<QueryResultValueItem *>(index.internalPointer());

                    if (!valueItem->isLocked())
                    {
                        property = static_cast<QueryResultPropertyItem *>(valueItem->parent());

                        if (m_container.removeValue(static_cast<QueryResultRootItem *>(property->parent())->value(), valueItem->value()))
                        {
                            idx = property->indexOf(valueItem);

                            beginRemoveRows(Model::parent(index), idx, idx);
                            property->remove(idx);
                            endRemoveRows();
                        }
                        else
                        {
                            QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
                            m_container.rollback();
                            return;
                        }
                    }
                }

        if (m_container.commit())
        {
            if (!files.isEmpty())
            {
                lock(files, tr("Scanning for remove..."));
                addTask(new ScanFilesTask(ModelEvent::ScanFilesForRemove, this, files), files);
            }
        }
        else
        {
            QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
            m_container.rollback();
        }
    }
    else
        QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
}

void QueryResultsNode::refresh(INodeView *view)
{
    beginRemoveRows(QModelIndex(), 0, m_items.size() - 1);
    m_reader.refresh();
    m_items.clear();
    endRemoveRows();
}

void QueryResultsNode::process(const QModelIndexList &list, const Functor &functor)
{
    QueryResultItem *item;

    for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
        if ((item = static_cast<QueryResultItem *>(list.at(i).internalPointer()))->isValue())
            functor(list.at(i), static_cast<QueryResultValueItem *>(item));
}

void QueryResultsNode::processRemove(const QModelIndexList &list, const Functor &functor)
{
    QueryResultItem *item;

    for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
        if ((item = static_cast<QueryResultItem *>(list.at(i).internalPointer()))->isValue())
            functor(list.at(i), static_cast<QueryResultValueItem *>(item));
}

void QueryResultsNode::doRemove(INodeView *view, const QModelIndex &index, QueryResultValueItem *value)
{
    if (m_container.transaction())
    {
        QueryResultPropertyItem *property = static_cast<QueryResultPropertyItem*>(value->parent());

        if (m_container.removeValue(static_cast<QueryResultRootItem *>(property->parent())->value(), value->value()))
            if (m_container.commit())
            {
                beginRemoveRows(Model::parent(index), index.row(), index.row());
                property->remove(index.row());
                endRemoveRows();
            }
            else
            {
                QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
                m_container.rollback();
            }
        else
        {
            QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
            m_container.rollback();
        }
    }
    else
    {
        QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
        m_container.rollback();
    }
}

void QueryResultsNode::scanUpdates(const BaseTask::Event *e)
{
    typedef const ScanFilesTask::UpdatesEvent *Event;
    typedef ScanFilesTask::UpdatesEvent *NotConstEvent;
    Event event = static_cast<Event>(e);

    update(const_cast<NotConstEvent>(event)->snapshot);
    taskHandled(event->task);
}

void QueryResultsNode::scanForRemove(const BaseTask::Event *e)
{
    typedef const ScanFilesTask::Event *Event;
    typedef ScanFilesTask::Event *NotConstEvent;
    Event event = static_cast<Event>(e);

    if (!event->canceled)
    {
        QString question;
        QStringList removed;
        QStringList folders;
        QStringList files;
        SnapshotItem *entry;

        for (Snapshot::const_iterator i = event->snapshot.begin(), end = event->snapshot.end(); i != end; ++i)
            if (entry = (*i).second)
            {
                if (entry->info()->isDir())
                    folders.push_back(entry->info()->fileName());
                else
                    files.push_back(entry->info()->fileName());
            }
            else
                removed.push_back((*i).first.as<QueryResultPathItem>()->info()->fileName());

        question = tr("Would you like to remove").
                append(QString::fromLatin1("\n\t")).
                append(tr("directories:")).append(QString::fromLatin1("\n\t\t")).
                append(folders.join(QString::fromLatin1("\n\t\t"))).

                append(QString::fromLatin1("\n\t---\n\t")).
                append(tr("files:")).append(QString::fromLatin1("\n\t\t")).
                append(files.join(QString::fromLatin1("\n\t\t"))).

                append(QString::fromLatin1("\n\t---\n\t")).
                append(tr("doesn't exist:")).append(QString::fromLatin1("\n\t\t")).
                append(removed.join(QString::fromLatin1("\n\t\t"))).

                append(QString::fromLatin1("\n\t---\n")).
                append(tr("it will free ").append(Tools::humanReadableSize(event->snapshot.totalSize())));

        if (QMessageBox::question(
                Application::mainWindow(),
                tr("Remove..."),
                question,
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            lock(event->snapshot, tr("Removing..."));
            resetTask(new PerformRemoveTask(this, event->snapshot), event->task);
            return;
        }
    }

    unlock(event->snapshot);
    removeAllTaskLinks(event->task);
}

void QueryResultsNode::performRemove(const BaseTask::Event *e)
{
    typedef const PerformRemoveTask::Event *Event;
    QueryResultPropertyItem::size_type idx;
    Event event = static_cast<Event>(e);
    QueryResultPropertyItem *property;
    QModelIndex modelIdx;

    if (m_container.transaction())
    {
        bool ok = true;

        for (Snapshot::const_iterator i = event->snapshot.begin(), end = event->snapshot.end(); i != end; ++i)
            if ((*i).second == NULL || (*i).second->isRemoved())
            {
                property = static_cast<QueryResultPropertyItem *>((*i).first->parent());

                if (m_container.removeValue(static_cast<QueryResultRootItem *>(property->parent())->value(), (*i).first.as<QueryResultRootPathValueItem>()->value()))
                {
                    idx = property->indexOf((*i).first.data());

                    beginRemoveRows(index(property), idx, idx);
                    property->remove(idx);
                    endRemoveRows();
                }
                else
                {
                    QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
                    m_container.rollback();
                    ok = false;
                    break;
                }
            }

        if (ok)
            if (!m_container.commit())
            {
                QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
                m_container.rollback();
            }
    }
    else
        QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));

    for (Snapshot::const_iterator i = event->snapshot.begin(), end = event->snapshot.end(); i != end; ++i)
        if ((*i).second && !(*i).second->isRemoved())
        {
            (*i).first.as<QueryResultValueItem>()->unlock();
            modelIdx = index((*i).first.data());
            emit dataChanged(modelIdx, modelIdx);
        }

    removeAllTaskLinks(event->task);
}

void QueryResultsNode::lock(const Snapshot &snapshot, const QString &reason)
{
    typedef QMap<QueryResultItem *, Union> Map;
    qint32 lastColumn = columnCount(QModelIndex()) - 1;
    QueryResultPropertyItem *property;
    Map map;

    for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
    {
        property = static_cast<QueryResultPropertyItem *>((*i).first->parent());
        map[property].add(property->indexOf((*i).first.data()));
        (*i).first.as<QueryResultValueItem>()->lock(reason);
    }

    for (Map::const_iterator i = map.constBegin(), end = map.constEnd(); i != end; ++i)
        for (Union::List::size_type q = 0, size = (*i).size(); q < size; ++q)
            emit dataChanged(createIndex((*i).at(q).top(), 0, i.key()->at((*i).at(q).top())),
                             createIndex((*i).at(q).bottom(), lastColumn, i.key()->at((*i).at(q).bottom())));
}

void QueryResultsNode::unlock(const Snapshot &snapshot)
{
    typedef QMap<QueryResultItem *, Union> Map;
    qint32 lastColumn = columnCount(QModelIndex()) - 1;
    QueryResultPropertyItem *property;
    Map map;

    for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
    {
        property = static_cast<QueryResultPropertyItem *>((*i).first->parent());
        map[property].add(property->indexOf((*i).first.data()));
        (*i).first.as<QueryResultValueItem>()->unlock();
    }

    for (Map::const_iterator i = map.constBegin(), end = map.constEnd(); i != end; ++i)
        for (Union::List::size_type q = 0, size = (*i).size(); q < size; ++q)
            emit dataChanged(createIndex((*i).at(q).top(), 0, i.key()->at((*i).at(q).top())),
                             createIndex((*i).at(q).bottom(), lastColumn, i.key()->at((*i).at(q).bottom())));
}

void QueryResultsNode::update(Snapshot &updates)
{
    typedef QMap<QueryResultItem *, Union> Map;
    qint32 lastColumn = columnCount(QModelIndex()) - 1;
    QueryResultPropertyItem *property;
    QueryResultItem *item;
    Map map;

    for (Snapshot::iterator i = updates.begin(), end = updates.end(); i != end; i = updates.erase(i))
    {
        item = (*i).first.as<QueryResultItem>();
        property = static_cast<QueryResultPropertyItem *>(item->parent());
        map[property].add(property->indexOf(item));

        if (item->size())
        {
            beginRemoveRows(Model::index(item), 0, item->size() - 1);
            static_cast<QueryResultRootPathValueItem *>(item)->update((*i).second);
            endRemoveRows();
        }
        else
            static_cast<QueryResultRootPathValueItem *>(item)->update((*i).second);

        static_cast<QueryResultValueItem *>(item)->unlock();
    }

    for (Map::const_iterator i = map.constBegin(), end = map.constEnd(); i != end; ++i)
        for (Union::List::size_type q = 0, size = (*i).size(); q < size; ++q)
            emit dataChanged(createIndex((*i).at(q).top(), 0, i.key()->at((*i).at(q).top())),
                             createIndex((*i).at(q).bottom(), lastColumn, i.key()->at((*i).at(q).bottom())));
}

QueryResultsNode::ItemsContainer::ItemsContainer() :
    TasksNode::Container()
{}

QueryResultsNode::ItemsContainer::size_type QueryResultsNode::ItemsContainer::size() const
{
    return m_container.size();
}

QueryResultsNode::ItemsContainer::Item *QueryResultsNode::ItemsContainer::at(size_type index) const
{
    return m_container.at(index).data();
}

QueryResultsNode::ItemsContainer::size_type QueryResultsNode::ItemsContainer::indexOf(Item *item) const
{
    VFS::Item::Holder holder(static_cast<VFS::Item *>(item));
    return m_container.indexOf(holder);
}

IDM_PLUGIN_NS_END
