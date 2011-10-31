#include "idmqueryresultitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultItem::QueryResultItem(IdmEntityValue *value, IdmItem *parent) :
	IdmListItem(parent),
	m_value(value)
{
	if (m_value->entity()->type() == Database::Composite)
	{
		IdmCompositeEntityValue::List values;

		for (IdmEntity::size_type i = 0, size = m_value->entity()->size(); i < size; ++i)
		{
			values = static_cast<IdmCompositeEntityValue*>(m_value.data())->values(m_value->entity()->at(i).entity);

			for (IdmCompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
				m_items.push_back(new QueryResultItem(values.at(i), this));
		}
	}
}

QVariant QueryResultItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}

IDM_PLUGIN_NS_END
