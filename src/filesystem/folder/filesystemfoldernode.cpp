#include "filesystemfoldernode.h"
#include "items/filesystemfoldernoderoot.h"
#include "items/filesystemfoldernodeentry.h"
#include "events/filesystemmodelevents.h"
#include "tasks/scan/updatefilestask.h"
#include "../filesystempluginsmanager.h"
#include "../../tools/rangeintersection.h"
#include "../../application.h"


FILE_SYSTEM_NS_BEGIN

FolderNode::FolderNode(const Info &info, Node *parent) :
	Node(parent),
	m_updating(false),
	m_proxy(this),
	m_delegate(&m_proxy)
{
	if (!info.isRoot())
		m_items.add(new FolderNodeRoot(info));

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

bool FolderNode::event(QEvent *e)
{
	switch (static_cast<ModelEvent::Type>(e->type()))
	{
		case ModelEvent::ListFiles:
		{
			e->accept();
			listEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::UpdateFiles:
		{
			e->accept();
			updateFilesEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForRemove:
		{
			e->accept();
			scanForRemoveEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::RemoveFilesCompleted:
		{
			e->accept();
			removeCompleteEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::RemoveFilesCanceled:
		{
			e->accept();
			removeCanceledEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForSize:
		{
			e->accept();
			scanForSizeEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForCopy:
		{
			e->accept();
			scanForCopyEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::CopyFilesCompleted:
		{
			e->accept();
			copyCompleteEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::CopyFilesCanceled:
		{
			e->accept();
			copyCanceledEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForMove:
		{
			e->accept();
			scanForMoveEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::QuestionAnswer:
		{
			e->accept();
			questionAnswerEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::UpdatePerformProgress:
		{
			e->accept();
			updateProgressEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		default:
			break;
	}

	return QAbstractItemModel::event(e);
}

int FolderNode::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
        return 0;
	else
    	return m_items.size();
}

int FolderNode::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant FolderNode::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<FolderNodeItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row()).item->data(index.column(), role);
}

Qt::ItemFlags FolderNode::flags(const QModelIndex &index) const
{
    if (index.isValid())
    	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
        return Qt::NoItemFlags;
}

QVariant FolderNode::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex FolderNode::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		return createIndex(row, column, m_items.at(row).item);
    else
        return QModelIndex();
}

QModelIndex FolderNode::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

bool FolderNode::exists() const
{
	if (isRoot())
		return true;
	else
		rootItem()->exists();
}

QString FolderNode::fileName() const
{
	if (isRoot())
		return QString();
	else
		rootItem()->fileName();
}

QString FolderNode::absolutePath() const
{
	if (isRoot())
		return QString();
	else
		rootItem()->absolutePath();
}

QString FolderNode::absoluteFilePath() const
{
	if (isRoot())
		return QString::fromLatin1("/");
	else
		rootItem()->absoluteFilePath();
}

QDateTime FolderNode::lastModified() const
{
	if (isRoot())
		return QDateTime();
	else
		rootItem()->lastModified();
}

void FolderNode::refresh()
{
	if (!isRoot())
		rootItem()->refresh();
}

void FolderNode::update()
{
	if (!isUpdating())
		updateFiles();
}

void FolderNode::activated(const QModelIndex &idx)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (!static_cast<FolderNodeItem*>(index.internalPointer())->isRoot() &&
		!static_cast<FolderNodeEntry*>(index.internalPointer())->isLocked())
	{
		FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(index.internalPointer());
		entry->fileInfo().refresh();

		if (entry->fileInfo().exists())
			if (entry->fileInfo().isDir())
				if (FileSystemTree *tree = static_cast<FileSystemTree*>(static_cast<FileSystemTree*>(m_currentFsTree)->subtree(entry)))
				{
					beginRemoveRows(QModelIndex(), 0, m_currentFsTree->size() - 1);
					if (!tree->isUpdating())
						update(tree);
					endRemoveRows();

					beginInsertRows(QModelIndex(), 0, tree->size() - 1);
					m_currentFsTree = tree;
					endInsertRows();
				}
				else
				{
					beginRemoveRows(QModelIndex(), 0, m_currentFsTree->size() - 1);
					tree = static_cast<FileSystemTree*>(m_currentFsTree);
					tree->setSubtree(entry, m_currentFsTree = new FileSystemTree(entry->fileInfo().absoluteFilePath(), tree));
					static_cast<FileSystemTree*>(m_currentFsTree)->setParentEntry(entry);
					endRemoveRows();

					beginInsertRows(QModelIndex(), 0, m_currentFsTree->size() - 1);
					list(m_currentFsTree);
					endInsertRows();
				}
			else
			{

			}
		else
		{
			FolderNodeEntry::size_type index = m_currentFsTree->indexOf(entry);
			beginRemoveRows(QModelIndex(), index, index);
			static_cast<FileSystemTree*>(m_currentFsTree)->remove(index);
			endRemoveRows();
		}
	}
}

void FolderNode::remove(Node *subnode)
{

}

void FolderNode::remove(const QModelIndex &index)
{

}

void FolderNode::copy(const QModelIndex &index, Node *destination)
{

}

void FolderNode::move(const QModelIndex &index, Node *destination)
{

}

void FolderNode::createFolder(const QString &name)
{

}

void FolderNode::createFile(const QString &name)
{

}

void FolderNode::view(QAbstractItemView *itemView)
{
	itemView->setModel(&m_proxy);
	itemView->setItemDelegate(&m_delegate);
}

Node *FolderNode::node(const QString &fileName, PluginsManager *plugins)
{
	Values::size_type index = m_items.indexOf(fileName);

	if (index != Values::InvalidIndex)
		return m_items.at(index).node;
	else
	{
		QString path;

		if (isRoot())
			path = QString(fileName).prepend(QChar('/'));
		else
			path = rootItem()->absoluteFilePath().append(QChar('/')).append(fileName);

		Info info(path);
		Node *res = plugins->node(info, this);

		if (res == 0)
			if (info.isFile())
				return this;
			else
				if (info.isDir())
				{
					beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
					m_items.add(new FolderNodeEntry(info), res = new FolderNode(info, this));
					endInsertRows();
				}

		return res;
	}
}

void FolderNode::list(FolderNodeItem *fileSystemTree)
{

}

void FolderNode::listEvent(const ModelEvent::Params *p)
{

}

void FolderNode::updateFiles()
{
	QScopedPointer<UpdateFilesTask::Params> params(new UpdateFilesTask::Params());
	params->node = this;
	params->list = makeChangeSet();

	setUpdating(true);
	Application::instance()->taskPool().handle(new UpdateFilesTask(params.take()));
}

void FolderNode::updateFilesEvent(const ModelEvent::Params *p)
{
	typedef const UpdateFilesTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	ChangesList list = params->updates;
	RangeIntersection updateRange(1);

	for (ChangesList::size_type i = 0; i < list.size();)
		if (list.at(i).type() == Change::Updated)
		{
			FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(list.at(i).entry());
			Values::size_type index = m_items.indexOf(entry);
			entry->update(list.at(i).info());
			updateRange.add(index, index);
			list.removeAt(i);
		}
		else
			if (list.at(i).type() == Change::Deleted)
			{
				removeEntry(m_items.indexOf(list.at(i).entry()));
				list.removeAt(i);
			}
			else
				++i;

	for (RangeIntersection::RangeList::size_type i = 0, size = updateRange.size(); i < size; ++i)
		emit dataChanged(createIndex(updateRange.at(i).top(), 0),
						 createIndex(updateRange.at(i).bottom(), columnCount() - 1));

	if (!list.isEmpty())
	{
		beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + list.size() - 1);
		for (ChangesList::size_type i = 0, size = list.size(); i < size; ++i)
			m_items.add(new FolderNodeEntry(list.at(i).info()));
		endInsertRows();
	}

	if (params->isLastEvent)
		setUpdating(false);
}

void FolderNode::removeEntry(FolderNodeItem *fileSystemTree, FolderNodeItem *entry)
{

}

void FolderNode::scanForRemove(FolderNodeItem *fileSystemTree, FolderNodeItem *entry)
{

}

void FolderNode::scanForRemoveEvent(const ModelEvent::Params *p)
{

}

void FolderNode::removeCompleteEvent(const ModelEvent::Params *p)
{

}

void FolderNode::removeCanceledEvent(const ModelEvent::Params *p)
{

}

void FolderNode::scanForSize(FolderNodeItem *fileSystemTree, FolderNodeItem *entry)
{

}

void FolderNode::scanForSizeEvent(const ModelEvent::Params *p)
{

}

void FolderNode::copyEntry(FolderNodeItem *fileSystemTree, FolderNodeItem *entry, Node *destination)
{

}

void FolderNode::scanForCopy(FolderNodeItem *fileSystemTree, FolderNodeItem *entry, Node *destination)
{

}

void FolderNode::moveEntry(FolderNodeItem *fileSystemTree, FolderNodeItem *entry, Node *destination)
{

}

void FolderNode::scanForMove(FolderNodeItem *fileSystemTree, FolderNodeItem *entry, Node *destination)
{

}

void FolderNode::scanForCopyEvent(const ModelEvent::Params *p)
{

}

void FolderNode::scanForMoveEvent(const ModelEvent::Params *p)
{

}

void FolderNode::copyCompleteEvent(const ModelEvent::Params *p)
{

}

void FolderNode::copyCanceledEvent(const ModelEvent::Params *p)
{

}

void FolderNode::questionAnswerEvent(const ModelEvent::Params *p)
{

}

void FolderNode::updateProgressEvent(const ModelEvent::Params *p)
{

}

QModelIndex FolderNode::index(int column, FolderNodeItem *item) const
{
	int index = m_items.indexOf(item);

	if (index != -1)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

void FolderNode::updateFirstColumn(FolderNodeItem *fileSystemTree, FolderNodeItem *entry)
{

}

void FolderNode::updateSecondColumn(FolderNodeItem *fileSystemTree, FolderNodeItem *entry)
{

}

void FolderNode::updateBothColumns(FolderNodeItem *fileSystemTree, FolderNodeItem *entry)
{

}

void FolderNode::removeEntry(Values::size_type index)
{

}

void FolderNode::removeEntry(const QModelIndex &index)
{

}

void FolderNode::refresh(FolderNodeItem *fileSystemTree)
{

}

void FolderNode::doRefresh()
{
	if (!isUpdating())
		updateFiles();
}

ChangesList FolderNode::makeChangeSet() const
{
	ChangesList list;

	if (isRoot())
	{
		list.reserve(m_items.size());

		for (Values::size_type i = 0, size = m_items.size(); i < size; ++i)
			list.push_back(Change(Change::NoChange, m_items.at(i).item));
	}
	else
	{
		list.reserve(m_items.size() - 1);

		for (Values::size_type i = 1, size = m_items.size(); i < size; ++i)
			list.push_back(Change(Change::NoChange, m_items.at(i).item));
	}

	return list;
}

FILE_SYSTEM_NS_END
