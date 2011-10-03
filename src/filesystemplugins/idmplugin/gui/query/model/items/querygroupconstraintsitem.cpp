#include "querygroupconstraintsitem.h"


IDM_PLUGIN_NS_BEGIN

QueryGroupConstraintsItem::QueryGroupConstraintsItem(IdmItem *parent) :
	IdmListItem(parent),
	m_value(QString::fromLatin1("(...)"))
{}

QVariant QueryGroupConstraintsItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		switch (column)
		{
			case 0: return m_value;
			case 1: break;
		}

	return QVariant();
}

IDM_PLUGIN_NS_END
