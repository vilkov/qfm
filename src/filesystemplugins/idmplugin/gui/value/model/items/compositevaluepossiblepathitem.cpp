#include "compositevaluepossiblepathitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValuePossiblePathItem::CompositeValuePossiblePathItem(const IdmEntityValue::Holder &value, const InfoItem *source, IdmItem *parent) :
	CompositeValuePathItem(value, parent),
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

void CompositeValuePossiblePathItem::open() const
{

}

IDM_PLUGIN_NS_END
