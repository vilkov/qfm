#include "idmqueryresultpathvalueitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPathValueItem::QueryResultPathValueItem(const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent) :
	QueryResultValueItem(value, parent),
	m_info(container->location(value->value().toString()), Info::None()),
	m_node(NULL)
{
	lock(tr("Updating..."));
}

FileTypeId QueryResultPathValueItem::id() const
{
	return m_info.id();
}

QIcon QueryResultPathValueItem::icon() const
{
	return m_info.icon();
}

QString QueryResultPathValueItem::name() const
{
	return m_info.name();
}

QString QueryResultPathValueItem::description() const
{
	return m_info.description();
}

bool QueryResultPathValueItem::isDir() const
{
	return m_info.isDir();
}

bool QueryResultPathValueItem::isFile() const
{
	return m_info.isFile();
}

bool QueryResultPathValueItem::isLink() const
{
	return m_info.isLink();
}

bool QueryResultPathValueItem::exists() const
{
	return m_info.exists();
}

IFileInfo::size_type QueryResultPathValueItem::fileSize() const
{
	return m_info.fileSize();
}

QString QueryResultPathValueItem::fileName() const
{
	return value()->value().toString();
}

QString QueryResultPathValueItem::absolutePath() const
{
	return m_info.absolutePath();
}

QString QueryResultPathValueItem::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString QueryResultPathValueItem::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime QueryResultPathValueItem::lastModified() const
{
	return m_info.lastModified();
}

int QueryResultPathValueItem::permissions() const
{
	return m_info.permissions();
}

void QueryResultPathValueItem::refresh()
{
	m_info.refresh();
}

QVariant QueryResultPathValueItem::data(qint32 column, qint32 role) const
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
				return value()->value();
	}

	return QVariant();
}

IDM_PLUGIN_NS_END
