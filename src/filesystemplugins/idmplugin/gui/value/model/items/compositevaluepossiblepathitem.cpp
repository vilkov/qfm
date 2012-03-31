#include "compositevaluepossiblepathitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValuePossiblePathItem::CompositeValuePossiblePathItem(const IdmEntityValue::Holder &value, const InfoItem *source, IdmItem *parent) :
	CompositeValueItem(parent),
	m_value(value),
	m_source(source)
{}

QVariant CompositeValuePossiblePathItem::data(qint32 column, qint32 role) const
{
	switch (role)
	{
		case Qt::EditRole:
		case Qt::DisplayRole:
			return m_value->value();
		case Qt::DecorationRole:
			return m_source->icon();
		case Qt::TextAlignmentRole:
			return Qt::AlignLeft;
	}

	return QVariant();
}

bool CompositeValuePossiblePathItem::isPath() const
{
	return true;
}

IDM_PLUGIN_NS_END
