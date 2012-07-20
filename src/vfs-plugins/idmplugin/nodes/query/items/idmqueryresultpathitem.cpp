#include "idmqueryresultpathitem.h"
#include "../../../interfaces/idminvalidfileinfo.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPathItem::QueryResultPathItem(IFileInfo::Holder &info, Base *parent) :
	QueryResultItem(parent),
	m_info(info.take()),
	m_node(NULL)
{}

QueryResultPathItem::QueryResultPathItem(const IFileContainer *container, const QString &fileName, Base *parent) :
	QueryResultItem(parent),
	m_info(new InvalidInfo(fileName)),
	m_node(NULL)
{}

QueryResultPathItem::Base *QueryResultPathItem::at(Base::size_type index) const
{
	return m_items.at(index).data();
}

QueryResultPathItem::Base::size_type QueryResultPathItem::size() const
{
	return m_items.size();
}

QueryResultPathItem::Base::size_type QueryResultPathItem::indexOf(Base *item) const
{
	QueryResultItem::Holder holder(static_cast<QueryResultPathItem *>(item));
	return m_items.indexOf(holder);
}

QVariant QueryResultPathItem::data(qint32 column, qint32 role) const
{
	switch (role)
	{
		case Qt::EditRole:
		case Qt::DisplayRole:
			return m_info->fileName().as<QString>();
		case Qt::DecorationRole:
			if (isLocked())
				return lockIcon();
			else
				return m_info->fileType()->icon();
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

IDM_PLUGIN_NS_END
