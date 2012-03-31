#include "compositevaluepossiblepathitem.h"
#include "../../../../../../application.h"


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
		case Qt::ToolTipRole:
			return m_source->name();
	}

	return QVariant();
}

void CompositeValuePossiblePathItem::open() const
{
	Application::desktopService()->open(m_source->container(), m_source);
}

IDM_PLUGIN_NS_END
