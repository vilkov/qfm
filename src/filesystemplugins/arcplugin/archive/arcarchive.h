#ifndef ARCARCHIVE_H_
#define ARCARCHIVE_H_

#include <QtCore/QString>
#include "../nodes/items/arcnodeitem.h"


ARC_PLUGIN_NS_BEGIN

class Archive
{
public:
	typedef QList<ArcNodeItem::Base *> Container;
	struct Contents
	{
		Contents() :
			extractedSize(0)
		{}

		Container items;
		QString lastError;
		qint64 extractedSize;
	};

public:
	static Contents read(const QString &fileName, const volatile bool &aborted, const volatile bool &canceled);
};

ARC_PLUGIN_NS_END

#endif /* ARCARCHIVE_H_ */
