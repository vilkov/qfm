#include "m3unode.h"
#include "items/m3uentry.h"
#include "items/m3uroot.h"
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>


M3U_PLUGIN_NS_BEGIN

M3uNode::M3uNode(const QFileInfo &info, Node *parent) :
	Node(m_items, parent),
	m_tag(QString::fromLatin1("#EXTINF:")),
	m_updating(false)
{
	m_items.container().push_back(new M3uRoot(info));
	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

int M3uNode::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
        return 0;
	else
    	return m_items.size();
}

int M3uNode::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant M3uNode::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<M3uItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags M3uNode::flags(const QModelIndex &index) const
{
    if (index.isValid())
    	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
        return Qt::NoItemFlags;
}

QVariant M3uNode::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		switch (section)
		{
			case 0:
			{
				return tr("Title");
				break;
			}
			case 1:
			{
				return tr("Length");
				break;
			}
		}

	return QVariant();
}

QModelIndex M3uNode::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex M3uNode::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

IFileInfo *M3uNode::info(const QModelIndex &idx) const
{
	return 0;
}

ICopyControl *M3uNode::createControl(INodeView *view) const
{
	return 0;
}

void M3uNode::contextMenu(const QModelIndexList &list, INodeView *view)
{

}

void M3uNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void M3uNode::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void M3uNode::rename(const QModelIndex &index, INodeView *view)
{

}

void M3uNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void M3uNode::remove(const QModelIndexList &list, INodeView *view)
{

}

void M3uNode::cancel(const QModelIndexList &list, INodeView *view)
{

}

void M3uNode::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void M3uNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void M3uNode::copy(const INodeView *source, INodeView *destination)
{

}

void M3uNode::move(const INodeView *source, INodeView *destination)
{

}

void M3uNode::search(const QModelIndex &index, INodeView *view)
{

}

void M3uNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

void M3uNode::refresh()
{
//	if (isUpdating())
//		return;
//	else
//		setUpdating(true);
//
//	QFile file(rootItem()->absoluteFilePath());
//
//	if (file.open(QFile::ReadOnly))
//	{
//		QString line;
//		QStringList list;
//		QTextStream stream(&file);
//		M3uNode::Container::Container items;
//
//		stream.setCodec(QTextCodec::codecForName("UTF-8"));
//		qDeleteAll(m_items.container());
//		m_items.container().clear();
//
//		while (!(line = stream.readLine()).isEmpty())
//			if (line.startsWith(m_tag))
//			{
//				list = line.mid(m_tag.size()).split(QChar(','));
//
//				if (list.size() == 2)
//				{
//					QFileInfo info(line = stream.readLine());
//
//					if (!info.isAbsolute())
//						info = QFileInfo(rootItem()->absoluteFilePath().append(QChar('/')).append(line));
//
//					items.push_back(new M3uEntry(info, list.at(0).toInt(), list.at(1).trimmed()));
//				}
//			}
//
//		beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + items.size() - 1);
//		m_items.container().append(items);
//		endInsertRows();
//	}
//
//	setUpdating(false);
}

QString M3uNode::title() const
{
	return QString();
}

QString M3uNode::location() const
{
	return QString();
}

QString M3uNode::location(const QString &fileName) const
{
	return QString();
}

::History::Entry *M3uNode::menuAction(QAction *action, INodeView *view)
{
	return NULL;
}

Node *M3uNode::viewChild(const QModelIndex &idx, QModelIndex &selected)
{
	return 0;
}

Node *M3uNode::viewChild(const QString &fileName, QModelIndex &selected)
{
	return 0;
}

M3uNode::Container::Container()
{}

M3uNode::Container::~Container()
{
	qDeleteAll(m_container);
}

M3uNode::Container::size_type M3uNode::Container::size() const
{
	return m_container.size();
}

M3uNode::Container::Item *M3uNode::Container::at(size_type index) const
{
	return m_container.at(index);
}

M3uNode::Container::size_type M3uNode::Container::indexOf(Item *item) const
{
	return -1;//m_items.indexOf(item);
}

M3U_PLUGIN_NS_END
