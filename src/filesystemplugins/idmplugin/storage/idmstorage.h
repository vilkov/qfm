#ifndef IDMSTORAGE_H_
#define IDMSTORAGE_H_

#include <sqlite3.h>
#include <QtCore/QList>
#include <QtCore/QFileInfo>
#include <QtGui/QApplication>
#include "entities/idmentity.h"
#include "entities/idmentityroot.h"
#include "../../../filesystem/filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IdmStorage
{
	Q_DECLARE_TR_FUNCTIONS(IdmStorage)

public:
	IdmStorage(const QFileInfo &storage);
	~IdmStorage();

	bool isValid() const { return m_valid; }
	const QString &lastError() const { return m_lastError; }

private:
	void loadEntities(sqlite3_stmt *statement, IdmEntityList *parent);

private:
	bool m_valid;
	sqlite3 *m_db;
	QFileInfo m_info;
	QString m_lastError;
	IdmEntityRoot m_entities;
};

FILE_SYSTEM_NS_END

#endif /* IDMSTORAGE_H_ */
