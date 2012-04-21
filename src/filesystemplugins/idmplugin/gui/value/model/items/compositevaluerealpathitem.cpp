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

QString CompositeValueRealPathItem::fileName() const
{
	return QString();
}

bool CompositeValueRealPathItem::isFile() const
{
	return false;
}

void CompositeValueRealPathItem::open() const
{

}

IDM_PLUGIN_NS_END
