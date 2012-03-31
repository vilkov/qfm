#include "compositevaluerealpathitem.h"


IDM_PLUGIN_NS_BEGIN

CompositeValueRealPathItem::CompositeValueRealPathItem(const IdmEntityValue::Holder &value, IdmItem *parent) :
	CompositeValuePathItem(value, parent)
{}

QVariant CompositeValueRealPathItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}

void CompositeValueRealPathItem::open() const
{

}

IDM_PLUGIN_NS_END
