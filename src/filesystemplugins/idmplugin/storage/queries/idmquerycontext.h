#ifndef IDMQUERYCONTEXT_H_
#define IDMQUERYCONTEXT_H_

#include <sqlite3.h>
#include <QtCore/QByteArray>
#include <QtCore/QString>
#include "../../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class QueryContext
{
public:
	QueryContext();
	~QueryContext();

	void operator=(const QueryContext &other);

protected:
	friend class IdmStorage;
	QueryContext(sqlite3 *db, const QByteArray &query);

private:
	QueryContext(const QueryContext &other);

private:
	QString m_lastError;
	QByteArray m_query;
	sqlite3_stmt *m_statement;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYCONTEXT_H_ */
