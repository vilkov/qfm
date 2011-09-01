#ifndef DBHANDLEHOLDER_H_
#define DBHANDLEHOLDER_H_

#include <sqlite3.h>
#include <QtCore/QSharedData>
#include "../../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class DbHandle : public QSharedData
{
public:
	DbHandle(sqlite3 *db) :
		QSharedData(),
		m_db(db)
	{}

	~DbHandle()
	{
		sqlite3_close(m_db);
	}

	operator sqlite3 *() { return m_db; }

private:
	sqlite3 *m_db;
};

typedef QExplicitlySharedDataPointer<DbHandle> DbHandleHolder;

IDM_PLUGIN_NS_END

#endif /* DBHANDLEHOLDER_H_ */
