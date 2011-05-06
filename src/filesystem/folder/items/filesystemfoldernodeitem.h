#ifndef FILESYSTEMITEMFOLDERNODEITEM_H_
#define FILESYSTEMITEMFOLDERNODEITEM_H_

#include <QtCore/QVariant>
#include "../../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeItem
{
	Q_DISABLE_COPY(FolderNodeItem)

public:
	FolderNodeItem()
	{}
	virtual ~FolderNodeItem()
	{}

	virtual QVariant data(qint32 column, qint32 role) const = 0;
	virtual bool isRoot() const = 0;

	virtual QString fileName() const = 0;
	virtual QString absolutePath() const = 0;
	virtual QString absoluteFilePath() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMITEMFOLDERNODEITEM_H_ */
