#include "idmdatabasetypes.h"
#include "idmentitiestable.h"
#include "idmpropertiestable.h"
#include <QtCore/QDateTime>


IDM_PLUGIN_NS_BEGIN

QByteArray Database::init()
{
	return EntitiesTable::create().append(';').append(PropertiesTable::create());
}

QByteArray Database::loadId(const QString &tableName)
{
	return QString::fromLatin1("select ID from %1 order by ID desc").arg(tableName).toUtf8();
}

QByteArray Database::savepoint(const QByteArray &baseName)
{
	return QByteArray(baseName).
			append(QDateTime::currentDateTime().toString(QString::fromLatin1("hh:mm:ss.zzz")).toUtf8());
}

QString Database::typeToString(EntityType type)
{
	switch (type)
	{
		case Composite:
		case Rating:
		case Int:      return QString::fromLatin1("int");
		case String:   return QString::fromLatin1("char(256)");
		case Date:     return QString::fromLatin1("date");
		case Time:     return QString::fromLatin1("time");
		case DateTime: return QString::fromLatin1("datetime");
		case Memo:     return QString::fromLatin1("text");
		case Path:     return QString::fromLatin1("char(1024)");
		default:       return QString();
	}
}

QString Database::valueToString(EntityType type, const QVariant &value)
{
	switch (type)
	{
		case Composite:
		case Rating:
		case Int:      return QString::number(value.toInt());
		case Memo:
		case Path:
		case String:   return value.toString();
		case Date:     return value.toDate().toString("'MM/dd/yyyy'");
		case Time:     return value.toTime().toString("'hh:mm:ss'");
		case DateTime: return value.toDateTime().toString("'hh:mm:ss MM/dd/yyyy'");
		default:       return QString();
	}
}

QString Database::idsToString(const IdsSet &ids)
{
	QString res;

	for (IdsSet::const_iterator it = ids.constBegin(), end = ids.constEnd(); it != end; ++it)
		res.append(QString::number(*it)).append(QChar(','));

	res.chop(1);
	return res;
}

QString Database::idsToString(const IdsList &ids)
{
	QString res;

	for (IdsList::size_type i = 0, size = ids.size(); i < size; ++i)
		res.append(QString::number(ids.at(i))).append(QChar(','));

	res.chop(1);
	return res;
}

IDM_PLUGIN_NS_END
