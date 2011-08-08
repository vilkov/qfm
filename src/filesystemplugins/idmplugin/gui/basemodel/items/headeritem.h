#ifndef HEADERITEM_H_
#define HEADERITEM_H_

#include <QString>
#include <QStringList>
#include <QVariant>
#include "modelitem.h"


class HeaderItem : public ModelItem
{
	Q_DISABLE_COPY(HeaderItem)

public:
	HeaderItem() :
		ModelItem()
	{}
	HeaderItem(const QString &headerColumns, ModelItem *parent = 0);
	HeaderItem(const QList<QVariant> &headerColumns, ModelItem *parent = 0);

	int columnCount() const;
	QVariant data(int column, int role) const;

	void setHeader(const QList<QVariant> &headerColumns);
private:
	QList<QVariant> itemData;
};

#endif /* HEADERITEM_H_ */
