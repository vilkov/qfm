#include "idmmessage.h"


IDM_PLUGIN_NS_BEGIN

IdmMessage::IdmMessage(const QString &message, IdmItem *parent) :
	IdmItem(parent),
	m_message(message)
{}

QVariant IdmMessage::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_message;
	else
		return QVariant();
}

bool IdmMessage::isRoot() const
{
	return false;
}

bool IdmMessage::isList() const
{
	return false;
}

bool IdmMessage::isMenuItem() const
{
	return false;
}

bool IdmMessage::isEntityItem() const
{
	return false;
}

IDM_PLUGIN_NS_END
