#include "filesystemfoldernode.h"
#include "items/filesystemfoldernoderoot.h"
#include "items/filesystemfoldernodeentry.h"
#include "../filesystempluginsmanager.h"
#include <QDebug>


FILE_SYSTEM_NS_BEGIN

FolderNode::FolderNode(const Info &info, Node *parent) :
	Node(parent)
{
	if (!info.isRoot())
		m_items.add(new FolderNodeRoot(info));
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

QString FolderNode::fileName() const
{
	if (isRoot())
		return QString();
	else
		m_items.at(0).item->fileName();
}

QString FolderNode::absolutePath() const
{
	if (isRoot())
		return QString();
	else
		m_items.at(0).item->absolutePath();
}

QString FolderNode::absoluteFilePath() const
{
	if (isRoot())
		return QString();
	else
		m_items.at(0).item->absoluteFilePath();
}

void FolderNode::refresh()
{

}

void FolderNode::activated(const QModelIndex &index)
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
	itemView->setModel(this);
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
			path = m_items.at(0).item->absoluteFilePath().append(QChar('/')).append(fileName);

		qDebug() << path;
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

QModelIndex FolderNode::index(int column, FolderNodeItem *item) const
{
	int index = m_items.indexOf(item);

	if (index != -1)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

FILE_SYSTEM_NS_END
