#ifndef IDMROOT_H_
#define IDMROOT_H_

#include "idminfoitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmRoot : public IdmInfoItem
{
public:
	IdmRoot(const QFileInfo &info, IdmItem *parent = 0) :
		IdmInfoItem(info, parent),
		m_label(QString::fromLatin1(".."))
	{}

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const
	{
		if (column == 0 && role == Qt::DisplayRole)
			return m_label;
		else
			return QVariant();
	}

private:
	QString m_label;
};

FILE_SYSTEM_NS_END

#endif /* IDMROOT_H_ */
