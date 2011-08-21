#ifndef IDMENTRY_H_
#define IDMENTRY_H_

#include <QtCore/QCoreApplication>
#include "idminfoitem.h"
#include "../../../filesystem/tools/filesystemcommontools.h"


FILE_SYSTEM_NS_BEGIN

class IdmEntry : public IdmInfoItem
{
	Q_DECLARE_TR_FUNCTIONS(IdmEntry)

public:
	IdmEntry(const Info &info, qint32 length, const QString &title, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	qint32 length() const { return m_length; }
	const QString &title() const { return m_title; }

private:
	bool m_locked;
	QString m_lockReason;
	qint32 m_length;
	QString m_title;
};

FILE_SYSTEM_NS_END

#endif /* IDMENTRY_H_ */
