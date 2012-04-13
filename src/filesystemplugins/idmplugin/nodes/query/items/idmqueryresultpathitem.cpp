#include "idmqueryresultpathitem.h"
#include "../../../../../filesystem/tasks/concrete/containers/filesysteminfolistitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPathItem::QueryResultPathItem(const InfoItem *item, Base *parent) :
	QueryResultItem(parent),
	m_info(*item)
{
	if (item->isDir())
		for (InfoListItem::size_type i = 0, size = static_cast<const InfoListItem *>(item)->size(); i < size; ++i)
			m_items.push_back(new QueryResultPathItem(static_cast<const InfoListItem *>(item)->at(i), this));
}

QueryResultPathItem::QueryResultPathItem(const IFileContainer *container, const QString &fileName, Base *parent) :
	QueryResultItem(parent),
	m_info(container->location(fileName), Info::None())
{}

QueryResultPathItem::~QueryResultPathItem()
{
	qDeleteAll(m_items);
}

QueryResultPathItem::Base *QueryResultPathItem::at(Base::size_type index) const
{
	return m_items.at(index);
}

QueryResultPathItem::Base::size_type QueryResultPathItem::size() const
{
	return m_items.size();
}

QueryResultPathItem::Base::size_type QueryResultPathItem::indexOf(Base *item) const
{
	return m_items.indexOf(static_cast<QueryResultItem *>(item));
}

QVariant QueryResultPathItem::data(qint32 column, qint32 role) const
{
	switch (role)
	{
		case Qt::EditRole:
		case Qt::DisplayRole:
			return m_info.fileName();
		case Qt::DecorationRole:
			if (isLocked())
				return lockIcon();
			else
				return m_info.icon();
		case Qt::TextAlignmentRole:
			return Qt::AlignLeft;
		case Qt::ToolTipRole:
			if (isLocked())
				return lockReason();
			else
				break;
	}

	return QVariant();
}

bool QueryResultPathItem::isRoot()
{
	return false;
}

bool QueryResultPathItem::isProperty()
{
	return false;
}

bool QueryResultPathItem::isValue()
{
	return false;
}

bool QueryResultPathItem::isPath()
{
	return true;
}

FileTypeId QueryResultPathItem::id() const
{
	return m_info.id();
}

QIcon QueryResultPathItem::icon() const
{
	return m_info.icon();
}

QString QueryResultPathItem::name() const
{
	return m_info.name();
}

QString QueryResultPathItem::description() const
{
	return m_info.description();
}

bool QueryResultPathItem::isDir() const
{
	return m_info.isDir();
}

bool QueryResultPathItem::isFile() const
{
	return m_info.isFile();
}

bool QueryResultPathItem::isLink() const
{
	return m_info.isLink();
}

bool QueryResultPathItem::exists() const
{
	return m_info.exists();
}

IFileInfo::size_type QueryResultPathItem::fileSize() const
{
	return m_info.fileSize();
}

QString QueryResultPathItem::fileName() const
{
	return m_info.fileName();
}

QString QueryResultPathItem::absolutePath() const
{
	return m_info.absolutePath();
}

QString QueryResultPathItem::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString QueryResultPathItem::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime QueryResultPathItem::lastModified() const
{
	return m_info.lastModified();
}

int QueryResultPathItem::permissions() const
{
	return m_info.permissions();
}

void QueryResultPathItem::refresh()
{
	m_info.refresh();
}

IDM_PLUGIN_NS_END
