#ifndef IDMQUERYREADER_H_
#define IDMQUERYREADER_H_

#include <sqlite3.h>
#include <QtCore/QString>
#include <QtCore/QSharedData>
#include "../../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class IdmQueryReader
{
private:
	struct Data : public QSharedData
	{
		Data(sqlite3_stmt *statement) :
			statement(statement)
		{}
		~Data()
		{
			sqlite3_finalize(statement);
		}

		sqlite3_stmt *statement;
	};

public:
	enum ColumnType
	{
		Integer = SQLITE_INTEGER,
		Float = SQLITE_FLOAT,
		Blob = SQLITE_BLOB,
		Null = SQLITE_NULL,
		Text = SQLITE_TEXT
	};

public:
	IdmQueryReader(sqlite3_stmt *statement) :
		m_data(new Data(statement))
	{}

	bool next() { return sqlite3_step(m_data->statement) == SQLITE_ROW; }
	ColumnType columnType(int column) const { return static_cast<ColumnType>(sqlite3_column_type(m_data->statement, column)); }

	double asDouble(int column) const { return sqlite3_column_double(m_data->statement, column); }
	qint32 asInt(int column) const { return sqlite3_column_int(m_data->statement, column); }
	qint64 asInt64(int column) const { return sqlite3_column_int64(m_data->statement, column); }
	QByteArray asBlob(int column) const { return QByteArray((const char *)sqlite3_column_blob(m_data->statement, column), sqlite3_column_bytes(m_data->statement, column)); }
	QString asText(int column) const { return QString::fromUtf8((const char *)sqlite3_column_text(m_data->statement, column)); }

private:
	QExplicitlySharedDataPointer<Data> m_data;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYREADER_H_ */
