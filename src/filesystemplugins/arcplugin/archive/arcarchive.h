#ifndef ARCARCHIVE_H_
#define ARCARCHIVE_H_

#include <QtCore/QString>
#include "items/arcarchiveentry.h"


ARC_PLUGIN_NS_BEGIN

class Archive
{
public:
	typedef QList<ArchiveEntry*> Container;
	struct Contents
	{
		Contents();

		Container items;
		QString lastError;
		qint64 extractedSize;
	};

public:
	static Contents read(const QString &fileName);
};

ARC_PLUGIN_NS_END

#endif /* ARCARCHIVE_H_ */
