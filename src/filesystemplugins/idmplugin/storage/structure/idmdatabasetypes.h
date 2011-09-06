#ifndef IDMDATABASETYPES_H_
#define IDMDATABASETYPES_H_

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

	static QByteArray init();
	static QString typeToString(EntityType type);
};

IDM_PLUGIN_NS_END

#endif /* IDMDATABASETYPES_H_ */
