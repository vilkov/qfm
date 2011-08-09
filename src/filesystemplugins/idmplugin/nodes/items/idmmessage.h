#ifndef IDMMESSAGE_H_
#define IDMMESSAGE_H_

#include <QtCore/QString>
#include "idmitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmMessage : public IdmItem
{
public:
	IdmMessage(const QString &message, IdmItem *parent = 0) :
		IdmItem(parent),
		m_message(message)
	{}

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const
	{
		if (column == 0 && role == Qt::DisplayRole)
			return m_message;
		else
			return QVariant();
	}
	virtual bool isRoot() const { return false; }
	virtual bool isList() const { return false; }
	virtual bool isMenuItem() const { return false; }

private:
	QVariant m_message;
};

FILE_SYSTEM_NS_END

#endif /* IDMMESSAGE_H_ */
