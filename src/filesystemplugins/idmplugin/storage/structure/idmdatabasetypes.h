#ifndef IDMDATABASETYPES_H_
#define IDMDATABASETYPES_H_

#include <QtCore/QSet>
#include <QtCore/QList>
#include <QtCore/QString>
#include "../../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

struct Database
{
	enum EntityType
	{
		Int        = 1,
		String     = 2,
		Date       = 3,
		Time       = 4,
		DateTime   = 5,
		Memo       = 6,
		Composite  = 7,
		Rating     = 8,
		Path       = 9
	};

	typedef unsigned int id_type;
	enum { InvalidId = (id_type)-1 };

	typedef QSet<id_type>  IdsSet;
	typedef QList<id_type> IdsList;

	static QByteArray init();
	static QByteArray loadId(const QString &tableName);
	static QString typeToString(EntityType type);
	static QString idsToString(const IdsSet &ids);
	static QString idsToString(const IdsList &ids);
};

IDM_PLUGIN_NS_END

#endif /* IDMDATABASETYPES_H_ */
