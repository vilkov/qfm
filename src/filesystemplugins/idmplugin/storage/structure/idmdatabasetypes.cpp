#include "idmdatabasetypes.h"
#include "idmentitiestable.h"
#include "idmpropertiestable.h"


IDM_PLUGIN_NS_BEGIN

QByteArray Database::init()
{
	return EntitiesTable::create().append(';').append(PropertiesTable::create());
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
	}
}

IDM_PLUGIN_NS_END
