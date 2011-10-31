#include "idmentityvalueitem.h"


IDM_PLUGIN_NS_BEGIN

IdmEntityValueItem::IdmEntityValueItem(IdmEntityValue *value, IdmItem *parent) :
	IdmEntityItem(value->entity(), parent),
	m_value(value)
{}

IdmEntityValueItem::~IdmEntityValueItem()
{
	delete m_value;
}

QVariant IdmEntityValueItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		switch (column)
		{
			case 0: return m_value->value();
		}

	return QVariant();
}

bool IdmEntityValueItem::isValueItem() const
{
	return true;
}

IDM_PLUGIN_NS_END
