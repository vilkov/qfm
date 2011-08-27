#include "m3unode.h"
#include "items/m3uentry.h"
#include "items/m3uroot.h"
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>


M3U_PLUGIN_NS_BEGIN

M3uNode::M3uNode(const QFileInfo &info, Node *parent) :
	Node(parent),
	m_tag(QString::fromLatin1("#EXTINF:")),
	m_updating(false)
{
	m_items.push_back(new M3uRoot(info));
	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

M3uNode::~M3uNode()
{
	qDeleteAll(m_items);
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

bool M3uNode::exists() const
{
	return rootItem()->exists();
}

QString M3uNode::fileName() const
{
	return rootItem()->fileName();
}

QString M3uNode::absolutePath() const
{
	return rootItem()->absolutePath();
}

QString M3uNode::absoluteFilePath() const
{
	return rootItem()->absoluteFilePath();
}

QString M3uNode::absoluteFilePath(const QString &fileName) const
{
	return rootItem()->absoluteFilePath(fileName);
}

QDateTime M3uNode::lastModified() const
{
	return rootItem()->lastModified();
}

void M3uNode::refresh()
{
	if (isUpdating())
		return;
	else
		setUpdating(true);

	QFile file(rootItem()->absoluteFilePath());

	if (file.open(QFile::ReadOnly))
	{
		QString line;
		ItemsList items;
		QStringList list;
		QTextStream stream(&file);

		stream.setCodec(QTextCodec::codecForName("UTF-8"));
		qDeleteAll(m_items);
		m_items.clear();

		while (!(line = stream.readLine()).isEmpty())
			if (line.startsWith(m_tag))
			{
				list = line.mid(m_tag.size()).split(QChar(','));

				if (list.size() == 2)
				{
					QFileInfo info(line = stream.readLine());

					if (!info.isAbsolute())
						info = QFileInfo(rootItem()->absoluteFilePath().append(QChar('/')).append(line));

					items.push_back(new M3uEntry(info, list.at(0).toInt(), list.at(1).trimmed()));
				}
			}

		beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + items.size() - 1);
		m_items.append(items);
		endInsertRows();
	}

	setUpdating(false);
}

void M3uNode::menuAction(QAction *action)
{

}

void M3uNode::createFile(const QModelIndex &index)
{

}

void M3uNode::createDirectory(const QModelIndex &index)
{

}

void M3uNode::rename(const QModelIndexList &list)
{

}

void M3uNode::remove(const QModelIndexList &list)
{

}

void M3uNode::cancel(const QModelIndexList &list)
{

}

void M3uNode::calculateSize(const QModelIndexList &list)
{

}

void M3uNode::pathToClipboard(const QModelIndexList &list)
{

}

void M3uNode::copy(const QModelIndexList &list, INode *destination)
{

}

void M3uNode::move(const QModelIndexList &list, INode *destination)
{

}

void M3uNode::switchTo(Node *node, const QModelIndex &selected)
{

}

Node *M3uNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

Node *M3uNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

M3U_PLUGIN_NS_END
