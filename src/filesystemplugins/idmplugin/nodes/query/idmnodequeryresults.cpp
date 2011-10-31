#include "idmnodequeryresults.h"
#include "items/idmqueryresultitem.h"


IDM_PLUGIN_NS_BEGIN

IdmNodeQueryResults::IdmNodeQueryResults(const IdmContainer &container, const Select &query, const Info &info, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
	m_container(container),
	m_reader(m_container, query),
	m_info(info)
{}

IdmNodeQueryResults::~IdmNodeQueryResults()
{

}

int IdmNodeQueryResults::columnCount(const QModelIndex &parent) const
{
	return m_reader.entity()->type() == Database::Composite ? 2 : 1;
}

void IdmNodeQueryResults::fetchMore(const QModelIndex &parent)
{
	IdmEntityValue *item;

	for (qint32 actualLimit = 0; actualLimit < PrefetchLimit; ++actualLimit)
		if (item = m_reader.next())
			m_items.push_back(new QueryResultItem(item));
		else
			break;
}

bool IdmNodeQueryResults::canFetchMore(const QModelIndex &parent) const
{
    return !parent.isValid() && !m_reader.eof();
}

bool IdmNodeQueryResults::isDir() const
{
	return false;
}

bool IdmNodeQueryResults::isFile() const
{
	return false;
}

bool IdmNodeQueryResults::exists() const
{
	return true;
}

QString IdmNodeQueryResults::fileName() const
{
	return m_info.fileName();
}

QString IdmNodeQueryResults::absolutePath() const
{
	return m_info.absolutePath();
}

QString IdmNodeQueryResults::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString IdmNodeQueryResults::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime IdmNodeQueryResults::lastModified() const
{
	return m_info.lastModified();
}

void IdmNodeQueryResults::refresh()
{

}

IFileInfo *IdmNodeQueryResults::info(const QModelIndex &idx) const
{
	return 0;
}

ICopyControl *IdmNodeQueryResults::createControl() const
{
	return 0;
}

void IdmNodeQueryResults::menuAction(QAction *action, INodeView *view)
{

}

void IdmNodeQueryResults::createFile(const QModelIndex &index)
{

}

void IdmNodeQueryResults::createDirectory(const QModelIndex &index)
{

}

void IdmNodeQueryResults::rename(const QModelIndexList &list)
{

}

void IdmNodeQueryResults::remove(const QModelIndexList &list)
{

}

void IdmNodeQueryResults::cancel(const QModelIndexList &list)
{

}

void IdmNodeQueryResults::calculateSize(const QModelIndexList &list)
{

}

void IdmNodeQueryResults::pathToClipboard(const QModelIndexList &list)
{

}

void IdmNodeQueryResults::copy(const QModelIndexList &list, INode *destination)
{

}

void IdmNodeQueryResults::move(const QModelIndexList &list, INode *destination)
{

}

QModelIndex IdmNodeQueryResults::rootIndex() const
{
	return QModelIndex();
}

QAbstractItemModel *IdmNodeQueryResults::proxyModel() const
{
	return const_cast<IdmNodeQueryResults*>(this);
}

QAbstractItemDelegate *IdmNodeQueryResults::itemDelegate() const
{
	return 0;
}

const INodeView::MenuActionList &IdmNodeQueryResults::menuActions() const
{
	return m_actions;
}

Node *IdmNodeQueryResults::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

Node *IdmNodeQueryResults::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

IDM_PLUGIN_NS_END
