#include "headeritem.h"


HeaderItem::HeaderItem(const QString &headerColumns, ModelItem *parent) :
	ModelItem(parent)
{
	QStringList list = headerColumns.split(QString("|"));
	for (int i = 0; i < list.size(); ++i)
		itemData << list[i];
}

HeaderItem::HeaderItem(const QList<QVariant> &headerColumns, ModelItem *parent) :
	ModelItem(parent)
{
	itemData = headerColumns;
}

int HeaderItem::columnCount() const
{
	return itemData.size();
}

QVariant HeaderItem::data(int column, int role) const
{
	if (role == Qt::DisplayRole)
		return itemData.value(column);
	else
		return QVariant();
}

void HeaderItem::setHeader(const QList<QVariant> &headerColumns)
{
	itemData = headerColumns;
}
