#include "idmquerycontext.h"


IDM_PLUGIN_NS_BEGIN

QueryContext::QueryContext() :
	m_statement(0)
{}

QueryContext::~QueryContext()
{
	sqlite3_finalize(m_statement);
}

void QueryContext::operator=(const QueryContext &other)
{
	m_statement = other.m_statement;
	const_cast<QueryContext&>(other).m_statement = 0;
}

QueryContext::QueryContext(sqlite3 *db, const QByteArray &query) :
	m_statement(0)
{}

IDM_PLUGIN_NS_END
