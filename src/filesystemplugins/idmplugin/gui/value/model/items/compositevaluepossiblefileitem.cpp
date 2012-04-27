#include "compositevaluepossiblefileitem.h"
#include "../../../../../../application.h"


IDM_PLUGIN_NS_BEGIN

CompositeValuePossibleFileItem::CompositeValuePossibleFileItem(const IdmEntityValue::Holder &value, const WrappedNodeItem *source, IdmItem *parent) :
	CompositeValuePathItem(value, parent),
	m_source(source)
{}

QVariant CompositeValuePossibleFileItem::data(qint32 column, qint32 role) const
{
	switch (role)
	{
		case Qt::EditRole:
		case Qt::DisplayRole:
			return m_value->value();
		case Qt::DecorationRole:
			return m_source->info()->fileType()->icon();
		case Qt::TextAlignmentRole:
			return Qt::AlignLeft;
		case Qt::ToolTipRole:
			return m_source->info()->fileType()->name();
	}

	return QVariant();
}

QString CompositeValuePossibleFileItem::fileName() const
{
	return m_source->info()->fileName();
}

bool CompositeValuePossibleFileItem::isFile() const
{
	return true;
}

void CompositeValuePossibleFileItem::open() const
{
	Application::desktopService()->open(m_source->container(), m_source->info().data());
}

IDM_PLUGIN_NS_END
