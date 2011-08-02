#ifndef IDMSTORAGE_H_
#define IDMSTORAGE_H_

#include <sqlite3.h>
#include <QtCore/QFileInfo>
#include <QtGui/QApplication>
#include "../../../filesystem/filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IdmStorage
{
	Q_DECLARE_TR_FUNCTIONS(IdmStorage)

public:
	enum Type
	{
		Int      = 1,
		String   = 2,
		Date     = 3,
		Time     = 4,
		DateTime = 5,
		Memo     = 6,
		Virtual  = 7
	};

public:
	IdmStorage(const QFileInfo &storage);
	~IdmStorage();

	bool isValid() const { return m_valid; }
	const QString &lastError() const { return m_lastError; }

private:
	bool m_valid;
	sqlite3 *m_db;
	QFileInfo m_info;
	QString m_lastError;
};

FILE_SYSTEM_NS_END

#endif /* IDMSTORAGE_H_ */
