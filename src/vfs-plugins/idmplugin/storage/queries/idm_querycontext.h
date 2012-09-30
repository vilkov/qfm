#ifndef IDM_QUERYCONTEXT_H_
#define IDM_QUERYCONTEXT_H_

#include <sqlite3.h>
#include <QtCore/QString>
#include <QtCore/QSharedData>
#include "../entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class QueryContext
{
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
	QueryContext()
	{}

	bool isValid() const { return m_data; }
	bool next() const { return sqlite3_step(m_data->statement) == SQLITE_ROW; }
	ColumnType columnType(int column) const { return static_cast<ColumnType>(sqlite3_column_type(m_data->statement, column)); }
	int columnCount() const { return sqlite3_column_count(m_data->statement); }
	Entity *entity() const { return m_data->entity; }

	double asDouble(int column) const { return sqlite3_column_double(m_data->statement, column); }
	qint32 asInt(int column) const { return sqlite3_column_int(m_data->statement, column); }
	qint64 asInt64(int column) const { return sqlite3_column_int64(m_data->statement, column); }
	QByteArray asBlob(int column) const { return QByteArray((const char *)sqlite3_column_blob(m_data->statement, column), sqlite3_column_bytes(m_data->statement, column)); }
	QString asText(int column) const { return QString::fromUtf8((const char *)sqlite3_column_text(m_data->statement, column)); }

private:
	struct Data : public QSharedData
	{
		Data(Entity *entity, sqlite3_stmt *statement) :
			entity(entity),
			statement(statement)
		{}
		~Data()
		{
			sqlite3_finalize(statement);
		}

		Entity *entity;
		sqlite3_stmt *statement;
	};

private:
	friend class Storage;
	QueryContext(Entity *entity, sqlite3_stmt *statement) :
		m_data(new Data(entity, statement))
	{}

private:
	QExplicitlySharedDataPointer<Data> m_data;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYCONTEXT_H_ */
