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

bool FolderNode::isDir() const
{
	return true;
}

bool FolderNode::isFile() const
{
	return false;
}

bool FolderNode::exists() const
{
	if (isRootNode())
		return true;
	else
		return rootItem()->exists();
}

QString FolderNode::fileName() const
{
	if (isRootNode())
		return QString();
	else
		return rootItem()->fileName();
}

QString FolderNode::absolutePath() const
{
	if (isRootNode())
		return QString();
	else
		return rootItem()->absolutePath();
}

QString FolderNode::absoluteFilePath() const
{
	if (isRootNode())
		return QString::fromLatin1("/");
	else
		return rootItem()->absoluteFilePath();
}

QDateTime FolderNode::lastModified() const
{
	if (isRootNode())
		return QDateTime();
	else
		return rootItem()->lastModified();
}

IFile *FolderNode::open(OpenMode mode, QString &error) const
{
	return 0;
}

IFileInfo *FolderNode::create(const QString &fileName, FileType type, QString &error) const
{
	return 0;
}

void FolderNode::refresh()
{
	if (!isRootNode())
		rootItem()->refresh();
}

void FolderNode::remove(const QModelIndexList &index)
{

}

void FolderNode::copy(const QModelIndexList &index, Node *destination)
{

}

void FolderNode::move(const QModelIndexList &index, Node *destination)
{

}

void FolderNode::createFolder(const QString &name)
{

}

void FolderNode::createFile(const QString &name)
{

}

void FolderNode::update()
{
	if (!isUpdating())
		updateFiles();
}

Node *FolderNode::subnode(const QModelIndex &idx, PluginsManager *plugins)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<FolderNodeItem*>(index.internalPointer())->isRootItem())
		return static_cast<Node*>(Node::parent());
	else
		if (!static_cast<FolderNodeEntry*>(index.internalPointer())->isLocked())
		{
			FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(index.internalPointer());
			entry->refresh();

			if (entry->exists())
			{
				Q_ASSERT(m_items.indexOf(entry) != Values::InvalidIndex);
				Values::Value &value = m_items[m_items.indexOf(entry)];

				if (value.node != 0)
					value.node->setParentEntryIndex(idx);
				else
					if (value.node = createNode(*value.item, plugins))
						value.node->setParentEntryIndex(idx);

				return value.node;
			}
			else
			{
				Values::size_type index = m_items.indexOf(entry);
				beginRemoveRows(QModelIndex(), index, index);
				m_items.remove(index);
				endRemoveRows();
			}
		}

	return 0;
}

void FolderNode::remove(Node *subnode)
{

}

void FolderNode::view(QAbstractItemView *itemView)
{
	itemView->setModel(&m_proxy);
	itemView->setItemDelegate(&m_delegate);
}

QModelIndex FolderNode::indexFor(const QString &fileName)
{
	Values::size_type index = m_items.indexOf(fileName);

	if (index == Values::InvalidIndex)
	{
		Info info = fileInfo(fileName);

		if (info.exists())
		{
			m_items.add(new FolderNodeEntry(info));
			return m_proxy.mapFromSource(createIndex(m_items.size() - 1, 0, m_items.last().item));
		}
	}
	else
		return m_proxy.mapFromSource(createIndex(index, 0, m_items.at(index).item));

	return QModelIndex();
}

QModelIndex FolderNode::rootIndex() const
{
	if (isRootNode())
		return QModelIndex();
	else
		return m_proxy.mapFromSource(createIndex(0, 0, rootItem()));
}

bool FolderNode::isRootIndex(const QModelIndex &index) const
{
	if (isRootNode())
		return false;
	else
		return static_cast<FolderNodeItem*>(m_proxy.mapToSource(index).internalPointer())->isRootItem();
}

Node *FolderNode::node(const QString &fileName, PluginsManager *plugins)
{
	Values::size_type index = m_items.indexOf(fileName);

	if (index == Values::InvalidIndex)
	{
		Node *res;
		m_items.add(createNode(fileName, plugins, res));
		return res;
	}
	else
	{
		Values::Value &value = m_items[index];

		if (value.node)
			return value.node;
		else
			return value.node = createNode(*value.item, plugins);
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
			(*entry) = list.at(i).info();
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

ChangesList FolderNode::makeChangeSet() const
{
	ChangesList list;

	if (isRootNode())
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

QModelIndex FolderNode::index(int column, FolderNodeItem *item) const
{
	int index = m_items.indexOf(item);

	if (index != -1)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

Node *FolderNode::createNode(const Info &info, PluginsManager *plugins) const
{
	if (Node *res = plugins->node(&info, (FolderNode*)this))
		return res;
	else
		if (info.isDir())
			return new FolderNode(info, (FolderNode*)this);
		else
			return 0;
}

FolderNode::Values::Value FolderNode::createNode(const QString &fileName, PluginsManager *plugins, Node *&node) const
{
	Info info = fileInfo(fileName);
	return Values::Value(new FolderNodeEntry(info), node = createNode(info, plugins));
}

Info FolderNode::fileInfo(const QString &fileName) const
{
	return Info(
			isRootNode() ?
				QString(fileName).prepend(QChar('/')) :
				rootItem()->absoluteFilePath().append(QChar('/')).append(fileName));
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

FILE_SYSTEM_NS_END
