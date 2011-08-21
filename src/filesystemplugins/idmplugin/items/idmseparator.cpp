#include "idmseparator.h"


FILE_SYSTEM_NS_BEGIN

IdmSeparator::IdmSeparator(IdmItem *parent) :
	IdmItem(parent)
{}

QVariant IdmSeparator::data(qint32 column, qint32 role) const
{
	return QVariant();
}

bool IdmSeparator::isRoot() const
{
	return false;
}

bool IdmSeparator::isList() const
{
	return false;
}

bool IdmSeparator::isMenuItem() const
{
	return false;
}

bool IdmSeparator::isEntityItem() const
{
	return false;
}

FILE_SYSTEM_NS_END
