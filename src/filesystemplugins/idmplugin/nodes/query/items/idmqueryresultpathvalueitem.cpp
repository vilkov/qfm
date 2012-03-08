#include "idmqueryresultpathvalueitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPathValueItem::QueryResultPathValueItem(const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent) :
	QueryResultValueItem(value, parent),
	m_info(container->location(value->value().toString())),
	m_node(NULL)
{}

QVariant QueryResultPathValueItem::data(qint32 column, qint32 role) const
{
	switch (role)
	{
		case Qt::EditRole:
		case Qt::DisplayRole:
			return info().fileName();
		case Qt::DecorationRole:
			if (isLocked())
				return lockIcon();
			else
				return info().icon();
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
